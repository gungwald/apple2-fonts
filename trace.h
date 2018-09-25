#ifndef APPLICATION_TRACE_HEADER
#define APPLICATION_TRACE_HEADER

#ifdef TRACE
#define TRACE_MSG(m,n) printf(m); printf(n)
#else
#define TRACE_MSG(m,n)
#endif

#endif

