/* Added by Martin Thomas */
#ifndef swi_h_
#define swi_h_

#ifdef __cplusplus
 extern "C" {
#endif

/* Prototypes for functions defined in swi_handler.S */
void IntEnable(void);
void IntDisable(void);

#ifdef __cplusplus
 }
#endif

#endif
