//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLITÉCNICO DO CÁVADO E DO AVE
//                          2021/2022
//             ENGENHARIA DE SISTEMAS INFORMÁTICOS
//                    VISÃO POR COMPUTADOR
//
//             [  DUARTE DUQUE - dduque@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define VC_DEBUG
#ifndef VC_H
#define VC_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                   ESTRUTURA DE UMA IMAGEM
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct
{
	unsigned char* data;
	int width, height;
	int channels;	  // Binário/Cinzentos=1; RGB=3
	int levels;		  // Binário=1; Cinzentos [1,255]; RGB [1,255]
	int bytesperline; // width * channels
} IVC;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                   ESTRUTURA DE UM BLOB (OBJECTO)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct
{
	int x, y, width, height; // Caixa Delimitadora (Bounding Box)
	int area;				 // �rea
	int xc, yc;				 // Centro-de-massa
	int perimeter;			 // Per�metro
	int label;				 // Etiqueta
} OVC;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                    PROTÓTIPOS DE FUNÇÕES
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// FUNÇÕES: ALOCAR E LIBERTAR UMA IMAGEM
IVC* vc_image_new(int width, int height, int channels, int levels);
IVC* vc_image_free(IVC* image);

// FUNÇÕES: LEITURA E ESCRITA DE IMAGENS (PBM, PGM E PPM)
IVC* vc_read_image(char* filename);
int vc_write_image(char* filename, IVC* image);

int vc_rgb_to_gray(IVC* src, IVC* dst);

int vc_gray_negative(IVC* srcdst);
int vc_RGB_negative(IVC* srcdst);

int MAXIMO(int n1, int n2);

int vc_gray_to_binary(IVC* src, IVC* dst, int threshold);
int vc_gray_to_binary_global_mean(IVC* src, IVC* dst);
int vc_midpoint(IVC* src, IVC* dst);

int vc_binary_dilate(IVC* src, IVC* dst, int kernel);
int vc_binary_erosion(IVC* src, IVC* dst, int kernel);

int vc_binary_open(IVC* src, IVC* dst, int kernelErosion, int kernelDilation);
int vc_binary_close(IVC* src, IVC* dst, int kernelDilation, int kernelErosion);

OVC* vc_binary_blob_labelling(IVC* src, IVC* dst, int* nlabels);
int vc_binary_blob_info(IVC* src, OVC* blobs, int nblobs);

int vc_gray_histogram_show(IVC* src, IVC* dst);
int vc_gray_histogram_equalization(IVC* srcdst);

int vc_gray_edge_prewitt(IVC* src, IVC* dst, float th);

int vc_rgb_to_hsv(IVC* src, IVC* dst);
int vc_binary_to_rgb(IVC* src, IVC* dst);

int vc_rgb_to_hsv(IVC* src, IVC* dst);
int vc_hsv_segmentation(IVC* src, IVC* dst, int hmin, int hmax, int smin, int smax, int vmin, int vmax);
int HSVFilter(IVC* srcImg, IVC* dstImg);

int vc_draw_bouding_box(IVC* src, IVC* dst, OVC* blobs, int labels);

int vc_draw_center_mass(IVC* src, IVC* dst, OVC* blobs, int labels);
int vc_convert_bgr_to_rgb(IVC* src, IVC* dst);

#endif /* VC.H */