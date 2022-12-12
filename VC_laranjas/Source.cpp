#include <iostream>
#include <string>
#include <opencv2\opencv.hpp>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\videoio.hpp>


extern "C" {
#include "vc.h"
}


int main(void) {
	// V�deo
	char videofile[20] = "video.avi";
	cv::VideoCapture capture;
	struct
	{
		int width, height;
		int ntotalframes;
		int fps;
		int nframe;
	} video;
	// Outros
	std::string str;
	int key = 0;

	/* Leitura de v�deo de um ficheiro */
	/* NOTA IMPORTANTE:
	O ficheiro video.avi dever� estar localizado no mesmo direct�rio que o ficheiro de c�digo fonte.
	*/
	capture.open(videofile);

	/* Em alternativa, abrir captura de v�deo pela Webcam #0 */
	//capture.open(0, cv::CAP_DSHOW); // Pode-se utilizar apenas capture.open(0);

	/* Verifica se foi poss�vel abrir o ficheiro de v�deo */
	if (!capture.isOpened())
	{
		std::cerr << "Erro ao abrir o ficheiro de v�deo!\n";
		return 1;
	}

	/* N�mero total de frames no v�deo */
	video.ntotalframes = (int)capture.get(cv::CAP_PROP_FRAME_COUNT);
	/* Frame rate do v�deo */
	video.fps = (int)capture.get(cv::CAP_PROP_FPS);
	/* Resolu��o do v�deo */
	video.width = (int)capture.get(cv::CAP_PROP_FRAME_WIDTH);
	video.height = (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT);

	/* Cria uma janela para exibir o v�deo */
	cv::namedWindow("VC - VIDEO", cv::WINDOW_AUTOSIZE);

	cv::Mat frame;
	int laranjas = 0;
	while (key != 'q') {
		/* Leitura de uma frame do v�deo */
		capture.read(frame);

		/* Verifica se conseguiu ler a frame */
		if (frame.empty()) break;

		/* N�mero da frame a processar */
		video.nframe = (int)capture.get(cv::CAP_PROP_POS_FRAMES);

		/* Exemplo de inser��o texto na frame */
		str = std::string("RESOLUCAO: ").append(std::to_string(video.width)).append("x").append(std::to_string(video.height));
		cv::putText(frame, str, cv::Point(20, 25), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 25), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
		str = std::string("TOTAL DE FRAMES: ").append(std::to_string(video.ntotalframes));
		cv::putText(frame, str, cv::Point(20, 50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
		str = std::string("FRAME RATE: ").append(std::to_string(video.fps));
		cv::putText(frame, str, cv::Point(20, 75), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 75), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
		str = std::string("N. DA FRAME: ").append(std::to_string(video.nframe));
		cv::putText(frame, str, cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);

		IVC* original = vc_image_new(video.width, video.height, 3, 255);
		IVC* imagem1 = vc_image_new(video.width, video.height, 3, 255);
		IVC* imagem2 = vc_image_new(video.width, video.height, 3, 255);
		IVC* imagem3 = vc_image_new(video.width, video.height, 1, 255);
		IVC* imagem4 = vc_image_new(video.width, video.height, 1, 255);
		IVC* imagem5 = vc_image_new(video.width, video.height, 1, 255);
		IVC* imagem6 = vc_image_new(video.width, video.height, 3, 255);

		memcpy(imagem1->data, frame.data, video.width* video.height * 3);
		memcpy(original->data, imagem1->data, video.width* video.height * 3);

		vc_convert_bgr_to_rgb(imagem1, imagem1);
		vc_rgb_to_hsv(imagem1, imagem2);

		//vc_hsv_segmentation(imagem2, imagem3, 15, 28, 70, 100, 10, 95);
		vc_hsv_segmentation(imagem2, imagem3, 15, 30, 85, 100, 31, 92);
		vc_binary_dilate(imagem3, imagem4, 5);

		//vc_hsv_segmentation(imagem2, imagem3, 13, 27, 85, 100, 25, 97);

		/*vc_hsv_segmentation(imagem2, imagem3, 18, 29, 83, 98, 25, 97);
		vc_binary_dilate(imagem3, imagem4, 9);*/


		//vc_binary_to_rgb(imagem4, imagem6);
		
	
		OVC* blobs = nullptr;
		int nblobs = 0;
		bool doBloobs = true;
		int labels = 0;
		
		if (doBloobs == true)
		{
			blobs = vc_binary_blob_labelling(imagem4, imagem5, &nblobs);
			vc_binary_blob_info(imagem5, blobs, nblobs);
		}

		memcpy(frame.data, original->data, video.width* video.height * 3);
		

		if (blobs != nullptr)
		{
			for (int b = 0;b < nblobs; b++)
			{
				if (blobs[b].area > 9750)
				{
					float raio = MAX((blobs[b].yc - blobs[b].y), (blobs[b].xc - blobs[b].x));
					cv::circle(frame, cv::Point(blobs[b].xc, blobs[b].yc), 6, cv::Scalar(0, 100, 250, 0), 1, 2, 0);
					cv::circle(frame, cv::Point(blobs[b].xc, blobs[b].yc), raio, cv::Scalar(100, 0, 250, 0), 2, 2, 0);

					str = std::string("Area: ").append(std::to_string(blobs[b].area));
					cv::putText(frame, str, cv::Point(blobs[b].xc+20, blobs[b].yc), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);

					str = std::string("Perimetro: ").append(std::to_string(blobs[b].perimeter));
					cv::putText(frame, str, cv::Point(blobs[b].xc+20, blobs[b].yc+50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);

					str = std::string("Diametro: ").append(std::to_string((raio*2)* 0.196));
					cv::putText(frame, str, cv::Point(blobs[b].xc + 20, blobs[b].yc + 100), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);

					if (blobs[b].yc >= 362 && blobs[b].yc <= 366) laranjas++;		

					float diametro = ((raio * 2) * 0.196);


					if (53 <= diametro && diametro < 60) {
						str = std::string("Calibre 13");
					}
					else if (56 <= diametro && diametro < 63) {
						str = std::string("Calibre 12");
					}
					else if (58 <= diametro && diametro < 66) {
						str = std::string("Calibre 11");
					}
					else if (60 <= diametro && diametro < 68) {
						str = std::string("Calibre 10");
					}
					else if (62 <= diametro && diametro < 70) {
						str = std::string("Calibre 9");
					}
					else if (64 <= diametro && diametro < 73) {
						str = std::string("Calibre 8");
					}
					else if (67 <= diametro && diametro < 76) {
						str = std::string("Calibre 7");
					}
					else if (70 <= diametro && diametro < 80) {
						str = std::string("Calibre 6");
					}
					else if (73 <= diametro && diametro < 84) {
						str = std::string("Calibre 5");
					}
					else if (77 <= diametro && diametro < 88) {
						str = std::string("Calibre 4");
					}
					else if (81 <= diametro && diametro < 92) {
						str = std::string("Calibre 3");
					}
					else if (84 <= diametro && diametro < 96) {
						str = std::string("Calibre 2");
					}
					else if (87 <= diametro && diametro <= 100) {
						str = std::string("Calibre 1");
					}
					else if (diametro > 100) {
						str = std::string("Calibre 0");
					}

					if (diametro >= 53.0) {
						cv::putText(frame, str, cv::Point(blobs[b].xc + 20, blobs[b].yc + 150), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
					}
				}
			}
		}
		
		//cv::rectangle(frame, cv::Point(0, 360), cv::Point(1280, 360), 2, 8, 0);
		
		str = std::string("Laranjas: ").append(std::to_string(laranjas));
		cv::putText(frame, str, cv::Point(700,680), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);

		/* Exibe a frame */
		cv::imshow("VC - VIDEO", frame);

		/* Sai da aplica��o, se o utilizador premir a tecla 'q' */
		key = cv::waitKey(1);
	}

	/* Fecha a janela */
	cv::destroyWindow("VC - VIDEO");

	/* Fecha o ficheiro de v�deo */
	capture.release();

	return 0;
}