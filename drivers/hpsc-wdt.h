#ifndef HPSC_WDT_H
#define HPSC_WDT_H

#include <stdint.h>
#include <stdbool.h>

struct hpsc_wdt;

typedef void (*hpsc_wdt_cb_t)(struct hpsc_wdt *wdt, unsigned stage, void *arg);

// If fewer stages than the timer HW supports are passed here,
// then the interrupts of latter stages are ignored (but they
// still happen, since all stages are always active in HW).
//
// Note: clk_freq_hz and max_div are fixed hardware characteristics of the
// board (but not of the IP block), i.e. what would be defined by the device
// tree node, hence they are not hardcoded in the driver. To divide the
// frequency, you would change the argument to wdt_configure, not here.
struct hpsc_wdt *wdt_create_monitor(const char *name, volatile uint32_t *base,
                                    hpsc_wdt_cb_t cb, void *cb_arg,
                                    uint32_t clk_freq_hz, unsigned max_div);
struct hpsc_wdt *wdt_create_target(const char *name, volatile uint32_t *base,
                                   hpsc_wdt_cb_t cb, void *cb_arg);
void wdt_destroy(struct hpsc_wdt *wdt);
int wdt_configure(struct hpsc_wdt *wdt, unsigned freq,
                  unsigned num_stages, uint64_t *timeouts);
uint64_t wdt_count(struct hpsc_wdt *wdt, unsigned stage);
uint64_t wdt_timeout(struct hpsc_wdt *wdt, unsigned stage);
bool wdt_is_enabled(struct hpsc_wdt *wdt);
void wdt_enable(struct hpsc_wdt *wdt);
void wdt_disable(struct hpsc_wdt *wdt);
void wdt_kick(struct hpsc_wdt *wdt);

void wdt_isr(struct hpsc_wdt *wdt, unsigned stage);

#endif // HPSC_WDT_H
