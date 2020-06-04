#ifndef __UTILITY_LOG_H
#define __UTILITY_LOG_H


/** HOW TO USE
 * 初期化はいらないので、 Log::verbose("%s\n", "hogehoeg"); とか
 * 
 * 
 */

class Log {
    public:
	Log();
	static void verbose(const char *format, ...);

    private:
	static char buffer[];
	static bool initialized;
};

#endif