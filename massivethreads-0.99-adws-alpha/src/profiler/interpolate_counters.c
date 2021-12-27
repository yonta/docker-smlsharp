/* 
 * complement_counters.c
 */

#define DAG_RECORDER 2
#include <dag_recorder.h>
#include <dag_recorder_impl.h>

/* linear interpolation.
   find the value at t based on 
   the value x0 at t0 and the value x1 at t1.

   t is typically but need not be between t0 and t1. */
static long long
interpolate_sample(unsigned long long t0, long long x0,
		   unsigned long long t1, long long x1,
		   unsigned long long t) {
  (void)dr_check(t1 > t0);
  double a = ((double)t - (double)t0) / ((double)t1 - (double)t0);
  double b = ((double)t1 - (double)t) / ((double)t1 - (double)t0);
  return (long long)(a * x1 + b * x0);
}

/* interpolate all counters of execution point q 
   based on those of q0 and q1.
   nc is the number of counters sampled. */
static void 
interpolate_point(dr_clock_pos * q0, dr_clock_pos * q1, 
		  dr_clock_pos * q, int nc) {
  int c;
  dr_clock_t t0 = q0->t;
  dr_clock_t t1 = q1->t;
  dr_clock_t t  = q->t;
  (void)dr_check(q0->t < q1->t);
  for (c = 0; c < nc; c++) {
    if (!q->counters[c]) {
      /* q's value is missing. interpolate it */
      long long x0 = q0->counters[c];
      long long x1 = q1->counters[c];
      //(void)dr_check(x0 <= x1);
      if (x0 > x1) {
        fprintf(stderr, "Warning: counter %d's value decreases from %lld (at t=%llu) to %lld (at t=%llu).\n",
                c,
                q0->counters[c], q0->t,
                q1->counters[c], q1->t);
      }
      q->counters[c] = interpolate_sample(t0, x0, t1, x1, t);
    }
  }
}

/* degenerate interpolation.
   use only q0 and assume q is the same
   as the last resort (when only one
   sample was taken) */
static void 
copy_point(dr_clock_pos * q0, dr_clock_pos * q, int nc) {
  int c;
  for (c = 0; c < nc; c++) {
    q->counters[c] = q0->counters[c];
  }
}

/* B : an array of pointers to dr_clock_pos.
   each dr_clock_pos essentially represents
   a point in execution, at either beginning 
   or end of an interval. it contains the
   timestamp and values of counters.
   E : the address at which B ends.

   B ... E are sorted (worker id, timestamp),
   so all pointers to intervals executed by 
   w are consecutivein the array.

   w : the id of the worker to work on
   nc : number of couters in each node.

   scan B and find an entry whose any counter is
   sampled.

*/

static dr_clock_pos ** 
next_sample_of_worker(dr_clock_pos ** B, dr_clock_pos ** E, int w, int nc, dr_clock_t t0) {
  dr_clock_pos ** p;
  for (p = B; p < E && (*p)->worker == w; p++) {
    dr_clock_pos * q = *p;
    if (q->t > t0) {
      int c;
      for (c = 0; c < nc; c++) {
        if (q->counters[c]) return p;
      }
    }
  }
  return 0;
}


#if 0
static void
get_counter_value_string(char * s, dr_clock_pos * p, int nc) {
  sprintf(s, "%lld", p->counters[0]);
  int c;
  for (c = 1; c < nc; c++) {
    sprintf(s, "%s/%lld", s, p->counters[c]);
  }
}
#endif

/*
   as the result of executing this function,
   interpolate counters of all points executed
   by worker w.
   
   this is done by scanning B until we find a
   point executed by another worker (presumably
   worker w + 1) or we reach B, and for each
   pointer found, interpolate or extrapolate its
   counters.

   we basically use two nearest points for
   interpolation, but a complication arises when
   (i) the point is near an either end of the
   array and thus is not enclosed by any two
   points, and (ii) there was only one sample
*/

static dr_clock_pos ** 
interpolate_worker(dr_clock_pos ** B, dr_clock_pos ** E, int w, int nc) {
  /* p0 is the next sample in B .. E. 
     0 if none is found */
  dr_clock_pos ** p0 = next_sample_of_worker(B, E, w, nc, 0);
  /* p0 is the next sample in B .. E.
     0 if p0 is the only sample */
  dr_clock_pos ** p1 
    = (p0 ? next_sample_of_worker(p0 + 1, E, w, nc, (**p0).t) : 0);
  dr_clock_pos ** p;
  (void)dr_check(!p0 || !p1 || (p1 != p0 && (*p1)->t > (*p0)->t));

  /* scan B */
  for (p = B; p < E && (*p)->worker == w; p++) {
#if 0
    dr_clock_pos temp_p = **p;
    int need_interpolation = 0;
    int c;
    for (c = 0; c < nc; c++) {
      if (temp_p.counters[c] == 0) {
        need_interpolation = 1;
        break;
      }
    }
    char counter_value_str[100];
    if (need_interpolation) {
      get_counter_value_string(counter_value_str, &temp_p, nc);
      fprintf(stderr, "sample (worker=%d,time=%llu,counters=%s) needs interpolation.. ",
              temp_p.worker,
              temp_p.t,
              counter_value_str);
    }
#endif
    if (p1) {
      /* we have two samples.
         use two samples to interpolate it */
      interpolate_point(*p0, *p1, *p, nc);
#if 0      
      if (need_interpolation) {
        get_counter_value_string(counter_value_str, *p, nc);
        fprintf(stderr, "interpolated with p1, counters=%s\n", counter_value_str);
      }
#endif        
      if (p == p1) {
        /* we reached the greater sample.
           try to find the next sample. */
        dr_clock_pos ** p2 = next_sample_of_worker(p1 + 1, E, w, nc, (**p1).t);
        (void)dr_check(p2 != p1);
        if (p2) {
          /* found. now shift the interval use for
             interpolation */
          p0 = p1;
          p1 = p2;
        }
        /* else (not found). i.e., p1 is the last sample.
           keep using (p0,p1) (extrapolation) */
      }
    } else if (p0) {
      copy_point(*p0, *p, nc);
#if 0
      if (need_interpolation) {
        get_counter_value_string(counter_value_str, *p, nc);
        fprintf(stderr, "interpolated with p0, counters=%s\n", counter_value_str);
      }
#endif
    }
    /* else, do nothing. counters are left as zeros */
  }
  return p;
}

static int
interpolate_all_workers(dr_clock_pos ** S, long n, int nw, int nc) {
  int w;
  dr_clock_pos ** E = S + n;
  for (w = 0; w < nw; w++) {
    S = interpolate_worker(S, E, w, nc);
  }
  return 1;
}

static int 
dr_clock_pos_cmp(const void * a_, const void * b_) {
  /* ap and bp pointing to array of pointers */
  dr_clock_pos * const * ap = a_;
  dr_clock_pos * const * bp = b_;
  dr_clock_pos * a = *ap;
  dr_clock_pos * b = *bp;
  if (a->worker < b->worker) 
    return -1;
  if (a->worker > b->worker) 
    return  1;
  if (a->t < b->t)
    return -1;
  if (a->t > b->t)
    return 1;
  //(void)dr_check(a_ == b_);
  return 0;
}

int 
interpolate_counters(dr_pi_dag * G) {
  /* make an array of pointers to elements of T */
  long n_nodes = G->n;
  long n_points = 2 * n_nodes; 	/* start/end of each node */
  dr_clock_pos ** S = dr_malloc(sizeof(dr_clock_pos*) * n_points);
  long i;
  /* collecct pointers to all start/end timestamp and counters */
  for (i = 0; i < n_nodes; i++) {
    S[2 * i]     = &G->T[i].info.start;
    S[2 * i + 1] = &G->T[i].info.end;
  }
  /* sort nodes in their start time (info.start.t) */
  qsort(S, n_points, sizeof(dr_clock_pos*), dr_clock_pos_cmp);
  /* now pointers in S are sorted in their.
     TODO: we always use dr_max_counters,
     which is waste of time. */
  interpolate_all_workers(S, n_points, G->num_workers, dr_max_counters);
  dr_free(S, sizeof(dr_clock_pos*) * n_points);
  return 1;
}
