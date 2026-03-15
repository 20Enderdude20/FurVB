inline int32_t Mpyhw(int32_t h, int32_t w) { // Thanks, pizzarollsroyce!
#ifdef __v810
	asm volatile("mpyhw %[h], %[w];" : [w] "=r"(w) : "0"(w), [h] "r"(h) : );
#else
	//VERIFY(h == (h << 15 >> 15));
	w = (h << 15 >> 15) * w;
#endif
	return w;
}

#define ENABLE_IRQS()  __asm__ volatile ("cli" ::: "memory")
#define DISABLE_IRQS()  __asm__ volatile ("sei" ::: "memory")