#include "QuoteConfig.h"

static QuoteConfig g_instance;

QuoteConfig * QuoteConfig::GetInstance()
{
	return &g_instance;
}
