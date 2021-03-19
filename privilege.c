// https://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html
#if defined __OBJC__ || defined __cplusplus
#error
#endif


// #include <stdio.h>
// #include <stdlib.h>
#include <assert.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>

#include "./privilege.h"

// su -c "id" mobile
// uid=501(mobile) gid=501(mobile) groups=501(mobile),250(_analyticsusers),299(systemusers)
#define NAME "mobile"
#define ID   501

#define eprintf(...) fprintf(stderr,__VA_ARGS__)

static void real_mobile_or_root(){
  // eprintf("gid %d egid %d\n",getgid(),getegid());
  // eprintf("uid %d euid %d\n",getuid(),geteuid());
  // exit(0);
  assert((getuid()==0 &&getgid()==0 )||
         (getuid()==ID&&getgid()==ID));
}

static void effective_root(){
  // setuid has no effect on egid
  assert(0==geteuid());
}

static void effective_mobile(){
  // setuid has no effect on egid
  assert(ID==geteuid());
}

void privilege_drop(){
  real_mobile_or_root();
  effective_root();
  // https://www.freebsd.org/cgi/man.cgi?query=getpwnam&format=html
  const struct passwd *const pw=getpwnam(NAME);
  assert(0==strcmp(pw->pw_name,NAME)&&
         pw->pw_uid==ID);
  // setuid executable has no permission to change egid
  assert(0==seteuid(pw->pw_uid));
  effective_mobile();
}

void privilege_escalate(){
  real_mobile_or_root();
  effective_mobile();
  // setuid executable has no permission to change egid
  assert(0==seteuid(0));
  effective_root();
}
