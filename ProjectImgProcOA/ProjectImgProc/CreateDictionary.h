#include "opencv\cv.h"
#include <opencv2\features2d.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/nonfree/nonfree.hpp>

using namespace cv;

static int _count_samples = 0;//#Used to detect at what sample we are!

public class Dictionary
{

	Mat _imageBlocks;//#Store the image blocks

	SiftDescriptorExtractor _detector, _extractor;//The SIFT Feature detector and extractor
	vector<KeyPoint> _keypoints; //Store the key-points that will be extracted by SIFT	
	Mat _descriptor; //Store the SIFT descriptors of images

//#Create Samples (Blocks)
public: void ExtractFeatures(Mat image, Size size, Mat samples){

	for (int x = 0; x < (image.rows - size.height); x += size.height)
	for (int y = 0; y < (image.cols - size.width); y += size.width)
	{
		_imageBlocks = image(cv::Rect(y, x, size.width, size.height)).clone();

		for (int z = 0; z < size.height; z++)
		for (int w = 0; w < size.width; w++)
			samples.at<float>(_count_samples, z *size.width + w) = _imageBlocks.at<uchar>(z, w);

		_count_samples++;
	}

}

//#Create Samples (SIFT)
public: void ExtractFeatures(Mat images, Mat samples){

	_detector.detect(images, _keypoints); //Detect feature key-points	
	//drawKeypoints(subDirImgArray, _keypoints, _keypointImages[w], Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	_extractor.compute(images, _keypoints, _descriptor); //Compute the descriptors for each key-point
	samples.push_back(_descriptor); //Put the all feature descriptors in a single Mat object 

}

/*public: void ClusterFeatures(Mat samples, Mat labels, int dictionarySize, Mat words){

	kmeans(samples, dictionarySize, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 100, 0.0001), 1, KMEANS_PP_CENTERS, words);

	//return words;
}*/

//#Save the Features in .Yml
public: void SaveFeaturesInYml(System::String^ nameYml, Mat words){

	std::string s = msclr::interop::marshal_as<std::string>(nameYml);//#Convert .yml name to std::string

	//#Store the dictionary
			//-----------------------
	FileStorage fs(s, FileStorage::WRITE);
	fs << "dictionary" << words;
	fs.release();

}

};