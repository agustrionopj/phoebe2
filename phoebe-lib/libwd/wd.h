#ifndef WD_H
#define WD_H 1

extern int atmx_(double *t, double *g, int *ifil, double *xintlog, double *xint);
extern int bbl_(double *rv, double *grx, double *gry, double *grz, double *rvq, double *grxq, double *gryq, double *grzq, int *mmsave, double *fr1, double *fr2, double *hld, double *slump1, double *slump2, double *theta, double *rho, double *aa, double *bb, double *phsv, double *pcsv, int *n1, int *n2, double *f1, double *f2, double *d__, double *hlum, double *clum, double *xh, double *xc, double *yh, double *yc, double *gr1, double *gr2, double *wl, double *sm1, double *sm2, double *tpolh, double *tpolc, double *sbrh, double *sbrc, double *tavh, double *tavc, double *alb1, double *alb2, double *xbol1, double *xbol2, double *ybol1, double *ybol2, double *phas, double *rm, double *xincl, double *hot, double *cool, double *snth, double *csth, double *snfi, double *csfi, double *tld, double *glump1, double *glump2, double *xx1, double *xx2, double *yy1, double *yy2, double *zz1, double *zz2, double *dint1, double *dint2, double *grv1, double *grv2, double *rftemp, double *rf1, double *rf2, double *csbt1, double *csbt2, double *gmag1, double *gmag2, double *glog1, double *glog2, double *fbin1, double *fbin2, double *delv1, double *delv2, double *count1, double *count2, double *delwl1, double *delwl2, double *resf1, double *resf2, double *wl1, double *wl2, double *dvks1, double *dvks2, double *tau1, double *tau2, double *emm1, double *emm2, double *hbarw1, double *hbarw2, double *xcl, double *ycl, double *zcl, double *rcl, double *op1, double *fcl, double *dens, double *encl, double *edens, double *taug, double *emmg, double *yskp, double *zskp, int *mode, int *iband, int *ifat1, int *ifat2, int *ifphn);
extern int binnum_(double *x, int *n, double *y, int *j);
extern int cloud_(double *cosa, double *cosb, double *cosg, double *x1, double *y1, double *z1, double *xc, double *yc, double *zc, double *rr, double *wl, double *op1, double *opsf, double *edens, double *acm, double *en, double *cmpd, double *ri, double *dx, double *dens, double *tau);
extern int conjph_(double *ecc, double *argper, double *phzero, double *trsc, double *tric, double *econsc, double *econic, double *xmsc, double *xmic, double *pconsc, double *pconic);
extern int dgmprd_(double *a, double *b, double *r__, int *n, int *m, int *l);
extern int dminv_(double *a, int *n, double *d__, int *l, int *m);
extern int dura_(double *f, double *xincl, double *rm, double *d__, double *the, double *omeg, double *r__);
extern int ellone_(double *ff, double *dd, double *rm, double *xl1, double *om1, double *xl2, double *om2);
extern int fourls_(double *th, double *ro, int *nobs, int *nth, double *aa, double *bb);
extern int gabs_(int *komp, double *smaxis, double *qq, double *ecc, double *period, double *dd, double *rad, double *xm, double *xmo, double *absgr, double *glog);
extern int jdph_(double *xjdin, double *phin, double *t0, double *p0, double *dpdt, double *xjdout, double *phout);
extern int kepler_(double *xm, double *ec, double *ecan, double *tr);
extern int lcr_(double *rv, double *grx, double *gry, double *grz, double *rvq, double *grxq, double *gryq, double *grzq, int *mmsave, double *fr1, double *fr2, double *hld, double *slump1, double *slump2, double *rm, double *poth, double *potc, int *n1, int *n2, double *f1, double *f2, double *d__, double *hlum, double *clum, double *xh, double *xc, double *yh, double *yc, double *gr1, double *gr2, double *sm1, double *sm2, double *tpolh, double *tpolc, double *sbrh, double *sbrc, int *ifat1, int *ifat2, double *tavh, double *tavc, double *alb1, double *alb2, double *xbol1, double *xbol2, double *ybol1, double *ybol2, double *vol1, double *vol2, double *snth, double *csth, double *snfi, double *csfi, double *tld, double *glump1, double *glump2, double *xx1, double *xx2, double *yy1, double *yy2, double *zz1, double *zz2, double *dint1, double *dint2, double *grv1, double *grv2, double *csbt1, double *csbt2, double *rftemp, double *rf1, double *rf2, double *gmag1, double *gmag2, double *glog1, double *glog2, int *mode, int *iband);
extern int legendre_(double *x, double *pleg, int *n);
extern int light_(double *phs, double *xincl, double *xh, double *xc, double *yh, double *yc, int *n1, int *n2, double *sumhot, double *sumkul, double *rv, double *grx, double *gry, double *grz, double *rvq, double *grxq, double *gryq, double *grzq, int *mmsave, double *theta, double *rho, double *aa, double *bb, double *slump1, double *slump2, double *somhot, double *somkul, double *d__, double *wl, double *snth, double *csth, double *snfi, double *csfi, double *tld, double *gmag1, double *gmag2, double *glog1, double *glog2, double *fbin1, double *fbin2, double *delv1, double *delv2, double *count1, double *count2, double *delwl1, double *delwl2, double *resf1, double *resf2, double *wl1, double *wl2, double *dvks1, double *dvks2, double *tau1, double *tau2, double *emm1, double *emm2, double *hbarw1, double *hbarw2, double *xcl, double *ycl, double *zcl, double *rcl, double *op1, double *fcl, double *edens, double *encl, double *dens, double *taug, double *emmg, double *yskp, double *zskp, int *iband, int *ifat1, int *ifat2, int *ifphn);
extern int linpro_(int *komp, double *dvks, double *hbarw, double *tau, double *emm, double *count, double *taug, double *emmg, double *fbin, double *delv);
extern int lum_(double *xlum, double *x, double *y, double *tpoll, int *n, int *n1, int *komp, double *sbr, double *rv, double *rvq, double *glump1, double *glump2, double *glog1, double *glog2, double *grv1, double *grv2, int *mmsave, double *summ, double *fr, double *sm, int *ifat, double *vol, double *rm, double *om, double *f, double *d__, double *snth, int *iband);
extern int lump_(double *grx, double *gry, double *grz, double *grxq, double *gryq, double *grzq, double *slump1, double *slump2, int *mmsave, double *alb, double *tpoll, double *sbr, int *n1, int *n2, int *komp, int *ifat, double *fr, double *snth, double *tld, double *glump1, double *glump2, double *xx1, double *xx2, double *yy1, double *yy2, double *zz1, double *zz2, double *xbol, double *ybol, double *grv1, double *grv2, double *sbr1b, double *sbr2b, double *rf, double *rfo, double *gmag1, double *gmag2, double *glog1, double *glog2, double *dint, int *iband);
extern int mlrg_(double *a, double *p, double *q, double *r1, double *r2, double *t1, double *t2, double *sm1, double *sm2, double *sr1, double *sr2, double *bolm1, double *bolm2, double *xlg1, double *xlg2);
extern int modlog_(double *rv, double *grx, double *gry, double *grz, double *rvq, double *grxq, double *gryq, double *grzq, int *mmsave, double *fr1, double *fr2, double *hld, double *rm, double *poth, double *potc, double *gr1, double *gr2, double *alb1, double *alb2, int *n1, int *n2, double *f1, double *f2, int *mod, double *xincl, double *the, int *mode, double *snth, double *csth, double *snfi, double *csfi, double *grv1, double *grv2, double *xx1, double *yy1, double *zz1, double *xx2, double *yy2, double *zz2, double *glump1, double *glump2, double *csbt1, double *csbt2, double *gmag1, double *gmag2, double *glog1, double *glog2);
extern int nekmin_(double *rm, double *omeg, double *x, double *z__);
extern int olump_(double *rv, double *grx, double *gry, double *grz, double *rvq, double *grxq, double *gryq, double *grzq, double *slump1, double *slump2, int *mmsave, double *grexp, double *alb, double *rb, double *tpoll, double *sbr, double *summ, int *n1, int *n2, int *komp, int *ifat, double *x, double *y, double *d__, double *snth, double *csth, double *snfi, double *csfi, double *tld, double *glump1, double *glump2, double *glog1, double *glog2, double *grv1, double *grv2, int *iband);
extern int planckint_(double *t, int *ifil, double *ylog, double *y);
extern int rangau_(double *smod, int *nn, double *sd, double *gau);
extern int ranuni_(double *sn, double *smod, double *sm1p1);
extern int rddata_(int *mpage, int *nref, int *mref, int *ifsmv1, int *ifsmv2, int *icor1, int *icor2, int *ld, int *jdphs, double *hjd0, double *period, double *dpdt, double *pshift, double *stdev, int *noise, double *seed, double *hjdst, double *hjdsp, double *hjdin, double *phstrt, double *phstop, double *phin, double *phn, int *mode, int *ipb, int *ifat1, int *ifat2, int *n1, int *n2, double *perr0, double *dperdt, double *the, double *vunit, double *e, double *a, double *f1, double *f2, double *vga, double *xincl, double *gr1, double *gr2, double *abunin, double *tavh, double *tavc, double *alb1, double *alb2, double *poth, double *potc, double *rm, double *xbol1, double *xbol2, double *ybol1, double *ybol2, int *iband, double *hlum, double *clum, double *xh, double *xc, double *yh, double *yc, double *el3, double *opsf, double *zero, double *factor, double *wl, double *binwm1, double *sc1, double *sl1, double *wll1, double *ewid1, double *depth1, int *kks, double *binwm2, double *sc2, double *sl2, double *wll2, double *ewid2, double *depth2, double *xlat, double *xlong, double *radsp, double *temsp, double *xcl, double *ycl, double *zcl, double *rcl, double *op1, double *fcl, double *edens, double *xmue, double *encl, int *lpimax, int *ispmax, int *iclmax);
extern int ring_(double *q, double *om, int *komp, int *l, double *fr, double *hld, double *r1, double *rl);
extern int romq_(double *omein, double *q, double *f, double *d__, double *ec, double *th, double *fi, double *r__, double *drdo, double *drdq, double *dodq, int *komp, int *mode);
extern int sincos_(int *komp, int *n, int *n1, double *snth, double *csth, double *snfi, double *csfi, int *mmsave);
extern int spot_(int *komp, int *n, double *sinth, double *costh, double *sinfi, double *cosfi, double *temf);
extern int square_(double *obs, int *nobs, int *ml, double *out, double *sd, double *xlamda, double *d__, double *cn, double *cnn, double *cnc, double *clc, double *ss, double *cl, int *ll, int *mm);
extern int surfas_(double *rmass, double *potent, int *n, int *n1, int *komp, double *rv, double *grx, double *gry, double *grz, double *rvq, double *grxq, double *gryq, double *grzq, int *mmsave, double *fr1, double *fr2, double *hld, double *ff, double *d__, double *snth, double *csth, double *snfi, double *csfi, double *grv1, double *grv2, double *xx1, double *yy1, double *zz1, double *xx2, double *yy2, double *zz2, double *csbt1, double *csbt2, double *glump1, double *glump2, double *gmag1, double *gmag2, double *glog1, double *glog2, double *grexp);
extern int volume_(double *v, double *q, double *p, double *d__, double *ff, int *n, int *n1, int *komp, double *rv, double *grx, double *gry, double *grz, double *rvq, double *grxq, double *gryq, double *grzq, int *mmsave, double *fr1, double *fr2, double *hld, double *snth, double *csth, double *snfi, double *csfi, double *summ, double *sm, double *grv1, double *grv2, double *xx1, double *yy1, double *zz1, double *xx2, double *yy2, double *zz2, double *csbt1, double *csbt2, double *glump1, double *glump2, double *gmag1, double *gmag2, double *glog1, double *glog2, double *grexp, int *ifc);
extern int wrhead_(int *ibef, int *nref, int *mref, int *ifsmv1, int *ifsmv2, int *icor1, int *icor2, int *ld, int *jdphs, double *hjd0, double *period, double *dpdt, double *pshift, double *stdev, int *noise, double *seed, double *hjdst, double *hjdsp, double *hjdin, double *phstrt, double *phstop, double *phin, double *phn, int *mode, int *ipb, int *ifat1, int *ifat2, int *n1, int *n2, double *perr0, double *dperdt, double *the, double *vunit, double *vfac, double *e, double *a, double *f1, double *f2, double *vga, double *xincl, double *gr1, double *gr2, int *nsp1, int *nsp2, double *abunin, double *tavh, double *tavc, double *alb1, double *alb2, double *phsv, double *pcsv, double *rm, double *xbol1, double *xbol2, double *ybol1, double *ybol2, int *iband, double *hlum, double *clum, double *xh, double *xc, double *yh, double *yc, double *el3, double *opsf, double *zero, double *factor, double *wl, double *binwm1, double *sc1, double *sl1, double *binwm2, double *sc2, double *sl2, double *wll1, double *ewid1, double *depth1, double *wll2, double *ewid2, double *depth2, int *kks, double *xlat, double *xlong, double *radsp, double *temsp, double *xcl, double *ycl, double *zcl, double *rcl, double *op1, double *fcl, double *edens, double *xmue, double *encl, double *dens, int *ns1, double *sms1, double *sr1, double *bolm1, double *xlg1, int *ns2, double *sms2, double *sr2, double *bolm2, double *xlg2, int *mmsave, double *sbrh, double *sbrc, double *sm1, double *sm2, double *phperi, double *pconsc, double *pconic, double *dif1, double *abunir, double *abun, int *mod);
extern int wrdata_(double *hjd, double *phas, double *yskp, double *zskp, double *htt, double *cool, double *total, double *tot, double *d__, double *smagg, double *vsum1, double *vsum2, double *vra1, double *vra2, double *vkm1, double *vkm2, double *delv1, double *delwl1, double *wl1, double *fbin1, double *resf1, double *delv2, double *delwl2, double *wl2, double *fbin2, double *resf2, double *rv, double *rvq, int *mmsave, int *ll1, int *lll1, int *llll1, int *ll2, int *lll2, int *llll2);
extern int wrfoot_(int *message, double *f1, double *f2, double *po, double *rm, double *f, double *dp, double *e, double *drdq, double *dodq, int *ii, int *mode, int *mpage);
extern int lc_(void);

#endif
