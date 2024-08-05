#ifndef SKE_SECURE_PORT_H
#define SKE_SECURE_PORT_H

#include <ske.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SKE_SECURE_PORT_FUNCTION
__attribute__((weak)) void ske_enable_secure_port(SKE_ALG alg, uint16_t sp_key_idx);
#endif

    
void ske_disable_secure_port(void);

#ifdef __cplusplus
}
#endif

#endif

