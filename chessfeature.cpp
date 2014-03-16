int main(int argc, char ** argv)
{
	if(argc != 2) return 0;
	Mat trainImg;
	trainImg = imread(argv[1], 1);
	HOGDescriptor *hog=new HOGDescriptor(cvSize(3,3),cvSize(3,3),cvSize(5,10),cvSize(3,3),9);
	vector<float>descriptors;
	hog->compute(trainImg, descriptors,Size(1,1), Size(0,0));
	printf("%d\n",descriptors.size());


}
