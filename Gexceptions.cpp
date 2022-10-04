#pragma once

#include "Gexcpetions.h"
#include <sstream>

Gexception::Gexception(int line, const char* file) noexcept:
	line(line),
	file(file)
{}


const char* Gexception::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Gexception::GetType() const noexcept {
	return "Program exception";
}
int Gexception::GetLine() const noexcept {
	return line;
}
const std::string& Gexception::GetFile() const noexcept {
	return file;
}

std::string Gexception::GetOriginString() const noexcept {
	std::ostringstream oss;
	oss << " [File] " << file << std::endl
		<< " [Line] " << line;
	return oss.str();
}