#ifndef NB_FXP_H
#define NB_FXP_H

#if   ((PV_CPU_ARCH_VERSION >=5) && (PV_COMPILER == EPV_ARM_GNUC))
    static inline Word32 amrnb_fxp_mac_16_by_16bb(Word32 L_var1, Word32 L_var2, Word32 L_var3)
{
        register Word32 ra = L_var1;
        register Word32 rb = L_var2;
        register Word32 rc = L_var3;
        Word32 result;

        __asm__ volatile("smlabb %0, %1, %2, %3"
             : "=r"(result)
                             : "r"(ra), "r"(rb), "r"(rc)
                            );
        return (result);
    }

    static inline Word32 amrnb_fxp_msu_16_by_16bb(Word32 L_var1, Word32 L_var2, Word32 L_var3)
{
        register Word32 ra = L_var1;
        register Word32 rb = L_var2;
        register Word32 rc = L_var3;
        Word32 result;

        __asm__ volatile("rsb %0, %1, #0"
             : "=r"(ra)
                             : "r"(ra)
                            );

        __asm__ volatile("smlabb %0, %1, %2, %3"
             : "=r"(result)
                             : "r"(ra), "r"(rb), "r"(rc)
                            );
        return (result);
    }
#else
    static inline Word32 amrnb_fxp_mac_16_by_16bb(Word32 L_var1, Word32 L_var2, Word32 L_var3)
    {
        Word32 result;

        result = L_var3 + L_var1 * L_var2;

        return result;
    }

    static inline Word32 amrnb_fxp_msu_16_by_16bb(Word32 L_var1, Word32 L_var2, Word32 L_var3)
    {
        Word32 result;

        result = L_var3 - L_var1 * L_var2;

        return result;
    }

#endif

#endif
