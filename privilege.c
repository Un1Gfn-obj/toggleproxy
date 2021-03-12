// https://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html
#if defined __OBJC__ || defined __cplusplus
#error
#endif

#include <assert.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>

// #include <stdio.h>
// #include <stdlib.h>

#include "./privilege.h"

#define NAME "mobile"
#define ID   501

static void r_mobile_or_root(){
  // assert(getgid()==0||getgid()==ID);
  assert(getuid()==0||getuid()==ID);
}

static void e_root(){
  // assert(ID==getegid());
  // fprintf(stderr,"%d\n",getegid());
  // abort();
  assert(0==geteuid());
}

static void e_mobile(){
  // assert(ID==getegid());
  assert(ID==geteuid());
}

void privilege_drop(){
  r_mobile_or_root();
  e_root();
  // https://www.freebsd.org/cgi/man.cgi?query=getpwnam&format=html
  const struct passwd *const pw=getpwnam(NAME);
  assert(
    0==strcmp(pw->pw_name,NAME)&&
    // pw->pw_gid==ID&&
    pw->pw_uid==ID
  );
  // assert(0==setegid(pw->pw_gid));
  assert(0==seteuid(pw->pw_uid));
  e_mobile();
}

void privilege_escalate(){
  r_mobile_or_root();
  e_mobile();
  // assert(0==setegid(0));
  assert(0==seteuid(0));
  e_root();
}
