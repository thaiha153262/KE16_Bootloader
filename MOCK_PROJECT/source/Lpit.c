#include "Lpit.h"
void LPIT0_IRQHandler(void) {
	/* clear flag */
	LPIT0->MSR = 0;
    flag_lpit_interrupt = 1;
    LPIT0->CLRTEN |= LPIT_CLRTEN_CLR_T_EN_0(1);
    LPIT0->MIER = LPIT_MIER_TIE0(0);
}
void LPIT_Init() {
	/* Disable FIRC */
//    SCG->FIRCCSR &= ~SCG_FIRCCSR_FIRCEN_MASK;
	SCG->CLKOUTCNFG &= ~(0xF << 24);
	SCG->CLKOUTCNFG |= (2 << 24U);
//	SCG->SIRCCFG |= (1U << 0);
	SCG->SIRCDIV |= (1 << 8U);
	/* Select clock source */
	PCC->CLKCFG[PCC_LPIT0_INDEX] |= (2U << 24U);
	/* Set FIRC divider */
	/* Enable clock for LPIT */
	PCC->CLKCFG[PCC_LPIT0_INDEX] |= PCC_CLKCFG_CGC_MASK;
	/* Reset the timer channel and register */
	LPIT0->MCR |= (LPIT_MCR_SW_RST_MASK);

	LPIT0->MCR &= ~(LPIT_MCR_SW_RST_MASK);
	/* Set debug in doze and debug mode */

	LPIT0->MCR |= LPIT_MCR_DBG_EN(1) | LPIT_MCR_DOZE_EN(1)|  (LPIT_MCR_M_CEN_MASK);
	/* 32-bit periodic count */
	/* Set interrupt */
	/* Set value to 1s */
	/* Enable interrupt */
    LPIT0->CHANNEL[0].TCTRL |= LPIT_TCTRL_MODE(0);
	LPIT0->CHANNEL[0].TVAL |= 8000000;
	LPIT0->MIER |= LPIT_MIER_TIE0_MASK;
	NVIC_EnableIRQ(LPIT0_IRQn);
}
