/*****************************************************************************/
/* asm_models.c - C models of the assembly routines, for testing on a PC.    */
/*                                                                           */
/* A PC cannot execute C6000 assembly, so these functions compute exactly    */
/* what FIR_filters_asm.asm and iir_filters_second_order_asm.asm compute     */
/* (2*h*x products, 32-bit saturation, rounding, >>16), bit for bit.         */
/*****************************************************************************/
#define N 51
typedef long long i64;
static short b1[N], b2[N], b3[N], b4[N];
static short iir_delay[3];

static i64 sat32(i64 v)
{ return v > 2147483647LL ? 2147483647LL : (v < -2147483648LL ? -2147483648LL : v); }

static void push(short *b, short x)          /* buffer[k+1] = buffer[k], buffer[0] = x */
{ int k; for (k = N - 2; k >= 0; k--) b[k + 1] = b[k]; b[0] = x; }

static i64 mac(const short *h, const short *b, int off, int L)   /* MAC ... MACR */
{
  i64 acc = 0; int i;
  for (i = 0; i < L - 1; i++) acc = sat32(acc + 2LL * h[off + i] * b[off + i]);
  acc = sat32(acc + 2LL * h[off + L - 1] * b[off + L - 1]);
  acc = sat32(acc + 0x8000);                                     /* rounding */
  return acc;
}

static int dual(const short *b, i64 acc)
{
  int y = (int)(acc >> 16);
  unsigned low = (unsigned)(b[(N - 1) / 2] - y) & 0xFFFFu;
  return (int)(((unsigned) y << 16) | low);
}

void  FIR_filters_asm_initialize(void)
{ int k; for (k = 0; k < N; k++) b1[k] = b2[k] = b3[k] = b4[k] = 0; }
short FIR_filter_asm  (const short *h, short x) { push(b1, x); return (short)(mac(h, b1, 0, N) >> 16); }
short FIR_filter_asm_2(const short *h, short x) { push(b2, x); return (short)(mac(h, b2, 0, N) >> 16); }
int   FIR_dual_filter_asm(const short *h, short x) { push(b3, x); return dual(b3, mac(h, b3, 0, N)); }
int   FIR_dual_filter_variable_asm(const short *h, short x, short length)
{
  int L = length, off;
  if (L < 3) L = 3;
  if (L > N) L = N;
  push(b4, x); off = (N - L) >> 1;
  return dual(b4, mac(h, b4, off, L));
}

void second_order_IIR_asm_initialise(void) { iir_delay[0] = iir_delay[1] = iir_delay[2] = 0; }
short second_order_IIR_direct_form_II_asm(const short *c, short x)
{
  short *d = iir_delay; i64 acc;
  acc = sat32(2LL * c[3] * x) >> 7;
  acc = sat32(acc - 2LL * c[4] * d[1]);
  acc = sat32(acc - 2LL * c[4] * d[1]);
  acc = sat32(acc - 2LL * c[5] * d[2]);
  d[0] = (short)(acc >> 16);
  acc = sat32(2LL * c[2] * d[2]);
  acc = sat32(acc + 2LL * c[1] * d[1]);
  acc = sat32(acc + 2LL * c[1] * d[1]);
  acc = sat32(acc + 2LL * c[0] * d[0]);
  d[2] = d[1]; d[1] = d[0];
  return (short)(acc >> 9);
}
