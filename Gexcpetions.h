#pragma once

#include <exception>
#include <winerror.h>
#include <string>

class Gexception : public std::exception {

public:
	Gexception(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;
private:
	int line;		//linea del error 
	std::string file;	//archivo de donde falló
protected:
	mutable std::string whatBuffer;
};