#ifndef HEADERGEN_HPP
#define HEADERGEN_HPP

#include <string>
#include <vector>

#define NB_INFOS 6

class HeaderGen{
private:
	enum idx_info{
		STATUS,
		LOC,
		TYPE,
		LENGTH,
		TRANSFER,
		CONTENT
	};

	std::vector<std::string> _Info;
	std::string _response;

public:
	HeaderGen() : _response(""){
		_Info.assign(NB_INFOS, "");
	}

	void setStatus(std::string str){
		_Info[STATUS].append("HTTP/1.1 ");
		_Info[STATUS].append(str);
		_Info[STATUS].append("\n");
	}

	void setType(std::string str){
		_Info[TYPE].append("Content-Type: ");
		_Info[TYPE].append(str);
		_Info[TYPE].append("\n");
	}
	

	void setLocation(std::string str){
		_Info[LOC].append("Location: ");
		_Info[LOC].append(str);
		_Info[LOC].append("\n");
	}

	void setTransEnc(std::string str){
		_Info[TRANSFER].append("Transfer-Encoding: ");
		_Info[TRANSFER].append(str);
		_Info[TRANSFER].append("\n");
	}

	void setContentString(std::string str){
		_Info[LENGTH].append("Content-Length: ");
		_Info[LENGTH].append(SSTR(str.length()));
		_Info[LENGTH].append("\n\n");
		_Info[CONTENT].append(str);
		_Info[CONTENT].append("\n");
	}
	
	void processResponse(){
		for (std::vector<std::string>::iterator it = _Info.begin(); it != _Info.end(); it++){
			if (it->size() > 0)
				_response.append(*it);
		}
	}

	bool checkStatus(){ return _Info[STATUS].empty(); }

	std::string getStatus(){ return _Info[STATUS]; }

	void clear(){ 
		_Info.clear(); 
		_response.clear(); 
		_Info.assign(NB_INFOS, "");
	}

	std::string getStr(){ return _response; }///log
};

#endif
