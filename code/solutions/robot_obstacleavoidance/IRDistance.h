#define AL 1195159
#define BL -1058
#define CL 10
#define IRmaxL 2552
int32_t LeftConvert(int32_t nl){
  if(nl < IRmaxL){
    return 800;
  }
  return AL/(nl + BL) + CL;
}

#define AC 1195159
#define BC -1058
#define CC 10
#define IRmaxC 2552
int32_t CenterConvert(int32_t nc){
  if(nc < IRmaxC){
    return 800;
  }
  return AC/(nc + BC) + CC;
}

#define AR 1195159
#define BR -1058
#define CR 10
#define IRmaxR 2552
int32_t RightConvert(int32_t nr){
  if(nr < IRmaxR){
    return 800;
  }
  return AR/(nr + BR) + CR;
}
