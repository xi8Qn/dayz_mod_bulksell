
// #define DEBUG_PRINT
#define ERROR_PRINT
#define INFO_PRINT

const string KAT_LOG_PREFIX = "Kat_BulkSell";

void Kat_DebugPrint(string msg)
{
#ifdef DEBUG_PRINT
    Print(string.Format("[%1] DEBUG >> %2", KAT_LOG_PREFIX, msg));
#endif
};

// void Kat_DebugPrintFormat(string fmt, void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL)
// {
// #ifdef DEBUG_PRINT
//     PrintFormat(fmt, param1, param2, param3, param4, param5, param6, param7, param8, param9);
// #endif
// };

void Kat_ErrorPrint(string msg)
{
#ifdef ERROR_PRINT
    Print(string.Format("[%1] ERROR >> %2", KAT_LOG_PREFIX, msg));
#endif
};

// void Kat_ErrorPrintFormat(string fmt, in void param1 = NULL, void param2 = NULL, void param3 = NULL, void param4 = NULL, void param5 = NULL, void param6 = NULL, void param7 = NULL, void param8 = NULL, void param9 = NULL)
// {
// #ifdef ERROR_PRINT
//     PrintFormat(fmt, param1, param2, param3, param4, param5, param6, param7, param8, param9);
// #endif
// };

void Kat_InfoPrint(string msg)
{
#ifdef INFO_PRINT
    Print(string.Format("[%1] INFO >> %2", KAT_LOG_PREFIX, msg));
#endif
};
