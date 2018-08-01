#ifndef _STRATEGY_TYPE
#define _STRATEGY_TYPE

enum class STRATEGY_TYPE
{
    START = 0,
    SEQUENTIAL = START,
    OPEN_MP,
    STD_THREAD,
    END
};

#endif