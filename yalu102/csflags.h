
#pragma once

#define CS_VALID                   0x0000001  /* dynamically valid */
#define CS_ADHOC                   0x0000002  /* ad hoc signed */
#define CS_GET_TASK_ALLOW          0x0000004  /* has get-task-allow entitlement */
#define CS_INSTALLER               0x0000008  /* has installer entitlement */
#define CS_HARD                    0x0000100  /* don't load invalid pages */
#define CS_KILL                    0x0000200  /* kill process if it becomes invalid */
#define CS_CHECK_EXPIRATION        0x0000400  /* force expiration checking */
#define CS_RESTRICT                0x0000800  /* tell dyld to treat restricted */
#define CS_ENFORCEMENT             0x0001000  /* require enforcement */
#define CS_REQUIRE_LV              0x0002000  /* require library validation */
#define CS_ENTITLEMENTS_VALIDATED  0x0004000

#define CS_ALLOWED_MACHO           0x00ffffe

#define CS_EXEC_SET_HARD           0x0100000  /* set CS_HARD on any exec'ed process */
#define CS_EXEC_SET_KILL           0x0200000  /* set CS_KILL on any exec'ed process */
#define CS_EXEC_SET_ENFORCEMENT    0x0400000  /* set CS_ENFORCEMENT on any exec'ed process */
#define CS_EXEC_SET_INSTALLER      0x0800000  /* set CS_INSTALLER on any exec'ed process */

#define CS_KILLED                  0x1000000  /* was killed by kernel for invalidity */
#define CS_DYLD_PLATFORM           0x2000000  /* dyld used to load this is a platform binary */
#define CS_PLATFORM_BINARY         0x4000000  /* this is a platform binary */
#define CS_PLATFORM_PATH           0x8000000  /* platform binary by the fact of path (osx only) */
