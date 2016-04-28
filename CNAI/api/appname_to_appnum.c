/* appname_to_appnum.c */

#include <cnaiapi.h>

/*-----------------------------------------------------------------------
 * appname_to_appnum - look up a port by service name
 *-----------------------------------------------------------------------
 */
appnum
appname_to_appnum(char *appname)
{

	struct servent		*sp;
	appnum			port;

	cnaiapi_init();

	pthread_mutex_lock(&appname_mutex);

	sp = getservbyname(appname, "tcp");
	if (sp == NULL) {
		pthread_mutex_unlock(&appname_mutex);
		return -1;
	}
	
	port = ntohs(sp->s_port);

	pthread_mutex_unlock(&appname_mutex);
	return port;
}
