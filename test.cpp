#include <sys/times.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "amrcc.h"

/*
 *#ifdef __cplusplus
 *extern "C" {
 *#endif
 *struct timeval		time_start;
 *struct timeval		time_end;
 *long		time_total = 0;
 *
 *#ifdef __cplusplus
 *}
 *#endif
 */

using namespace std;

int encodefile(const std::string& infile,const std::string& outfile) {
	ifstream ifs;
	ifs.open(infile.c_str(),ios::in | ios::binary);

	if( ifs.fail() ) {
		cout << "can not open file " << infile << endl;
		return -1;
	}

	ofstream ofs;
	ofs.open(outfile.c_str(),ios::out | ios::binary);
	if( ofs.fail() ) {
		cout << "can not create file " << outfile << endl;
		return -1;
	}
	
	unsigned char* amr = new unsigned char[1024*100];

	std::vector<unsigned char> pcm;
	typedef std::istreambuf_iterator<char> FsIt;
	std::copy(FsIt(ifs),FsIt(),std::back_inserter(pcm));

	size_t nFrame = pcm.size()/320;

	void* ct = amr_encoder_init(4,4,1);
	struct timeval start;
	gettimeofday(&start,NULL);
	int r = amr_encoder_encode(ct,(short*)&(pcm[0]),pcm.size()/sizeof(short),amr,1024*100);
	struct timeval end;
	gettimeofday(&end,NULL);
	long msUse = ( (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec)/1000 );
	cout << "encode " << nFrame << " frames for " << nFrame * 20 << "ms, use " << msUse << "ms" <<
	   " average " << (float)msUse/nFrame << "ms/frame" <<	endl;
		if( r > 0 ) {
			ofs.write((char*)amr,r);
		} else {
			cout << "no data output at frame " << nFrame << endl;
		}
	amr_encoder_uninit(ct);
	ifs.close();
	ofs.close();
	return 0;
}

int decodefile(const std::string& infile,const std::string& outfile) {
	ifstream ifs;
	ifs.open(infile.c_str(),ios::in | ios::binary);

	if( ifs.fail() ) {
		cout << "can not open file " << infile << endl;
		return -1;
	}

	ofstream ofs;
	ofs.open(outfile.c_str(),ios::out | ios::binary);
	if( ofs.fail() ) {
		cout << "can not create file " << outfile << endl;
		return -1;
	}
	
	std::vector<unsigned char> bits;
	typedef std::istreambuf_iterator<char> FsIt;
	std::copy(FsIt(ifs),FsIt(),std::back_inserter(bits));
	if( !bits.empty() ) {
		void* ct = amr_decoder_init();
		const int words = amr_decoder_max_decode_sample(&(bits[0]),bits.size());
		short* pcm_buf = new short[words];

		int samples = amr_decoder_decode(ct,&(bits[0]),bits.size(),pcm_buf,words);
		if( samples > 0 ) {
			ofs.write((char*)pcm_buf,sizeof(short)*samples);
		}
		delete [] pcm_buf;
		amr_decoder_uninit(ct);
	}
	
	ifs.close();
	ofs.close();
	return 0;
}


int main(int argc,char* argv[]) {
	if( argc < 4 ) {
		cout << "Usage: " << argv[0] << " e speechfile bitstreamfile" << endl;
                cout << "or   : " << argv[0] << " d bitstreamfile speechfile" << endl;

		return 0;
	}

	if( 'd' == argv[1][0] ) {
		decodefile(argv[2],argv[3]);
	} else if( 'e' == argv[1][0] ) {
		encodefile(argv[2],argv[3]);
	} else {
		std::cout << "unknown opt" << std::endl;
	}
	return 0;
}

