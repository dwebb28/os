/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _RPC_H_RPCGEN
#define _RPC_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct rpc_data {
	char content[100];
};
typedef struct rpc_data rpc_data;

#define COUNT_NUMBERS 0x12344
#define COUNT_VERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define count 1
extern  int * count_1(rpc_data *, CLIENT *);
extern  int * count_1_svc(rpc_data *, struct svc_req *);
extern int count_numbers_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define count 1
extern  int * count_1();
extern  int * count_1_svc();
extern int count_numbers_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_rpc_data (XDR *, rpc_data*);

#else /* K&R C */
extern bool_t xdr_rpc_data ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_RPC_H_RPCGEN */