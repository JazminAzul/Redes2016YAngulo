/* cname_to_comp.c */

#include <cnaiapi.h>

/*-----------------------------------------------------------------------
 * cname_to_comp - look up a host by name and return its IP address
 *-----------------------------------------------------------------------
 */
computer
cname_to_comp(char *cname)
{
	computer		c;
	struct hostent		*hp;

	cnaiapi_init();

	pthread_mutex_lock(&cname_mutex);


	hp = gethostbyname(cname);
	if (hp == NULL) {
		pthread_mutex_unlock(&cname_mutex);
		return -1;
	}
	
	if (hp->h_addrtype != AF_INET ||
	    hp->h_length != 4){//sizeof(computer)) {
		pthread_mutex_unlock(&cname_mutex);
		return -1;
	}
	  
	c = *((long *) hp->h_addr);

	pthread_mutex_unlock(&cname_mutex);

	return c;
}
