#include <stdlib.h>
#include <pwd.h>
#include "nb_assert.h"

int
getpwent_r(struct passwd *pwd, char *buffer, size_t buflen,
    struct passwd **result)
{
        int     r, retval;

        static const ns_dtab dtab[] = {
                NS_FILES_CB(_files_getpwent_r, NULL)
                NS_DNS_CB(_dns_getpwent_r, NULL)
                NS_NIS_CB(_nis_getpwent_r, NULL)
                NS_COMPAT_CB(_compat_getpwent_r, NULL)
                NS_NULL_CB
        };

        _DIAGASSERT(pwd != NULL);
        _DIAGASSERT(buffer != NULL);
        _DIAGASSERT(result != NULL);

        *result = NULL;
        retval = 0;
        mutex_lock(&_pwmutex);
        r = nsdispatch(NULL, dtab, NSDB_PASSWD, "getpwent_r", __nsdefaultcompat,
            &retval, pwd, buffer, buflen, result);
        mutex_unlock(&_pwmutex);
        switch (r) {
        case NS_SUCCESS:
        case NS_NOTFOUND:
                return 0;
        default:
                return retval;
        }
}
