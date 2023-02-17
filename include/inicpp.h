#ifndef INICPP_H_
#define INICPP_H_

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <assert.h>
#include <sstream>
#include <fstream>


namespace ini {

	/** returns 一串空白字符串 **/
	constexpr const char* whitespaces() {
		return " \t\n\r\f\v";
	}
	/* *返回缩进字符字符串* */
	constexpr const char* indents() {
		return " \t";
	}

	/** 将一个字符串的前后空白全删除
	  * @param str string to be trimmed in place */
	inline void trim(std::string &str) {
		// 首先从末端擦除应该更有效一些，因为从头擦除可能会将所有字符的多个索引移到前面。
		size_t lastpos = str.find_last_not_of(whitespaces());
		// 代表str全是这些空白符号
		if (lastpos == std::string::npos) {
			str.clear();
			return;
		}
		str.erase(lastpos + 1); // 先把最后面的空白全删了
		str.erase(0, str.find_first_not_of(whitespaces()));  // 再把最前面的空白删了
	}

	/************************************************
	 * Conversion Functors
	 ************************************************/

	inline bool strToLong(const std::string &value, long &result) {
		char *endptr;
		// check if decimal
		result = std::strtol(value.c_str(), &endptr, 10);
		if (*endptr == '\0') 
			return true;
		// check if octal
		result = std::strtol(value.c_str(), &endptr, 8);
		if (*endptr == '\0')
			return true;
		// check if hex
		result = std::strtol(value.c_str(), &endptr, 16);
		if (*endptr == '\0')
			return true;

		return false;
		
		/*
		// c++的写法,,应该也是一个意思
		size_t *end;
		result = std::stol(value, end, 8);  
		if (*end == 0) { return true; }
		*/
	}
	inline bool strToULong(const std::string &value, unsigned long &result) {
		char *endptr;
		// check if decimal
		result = std::strtoul(value.c_str(), &endptr, 10);
		if (*endptr == '\0')
			return true;
		// check if octal
		result = std::strtoul(value.c_str(), &endptr, 8);
		if (*endptr == '\0')
			return true;
		// check if hex
		result = std::strtoul(value.c_str(), &endptr, 16);
		if (*endptr == '\0')
			return true;

		return false;
	}

	// 先声明一个模板结构体？
	template<typename T>
	struct Convert {};

	template<>
	struct Convert<bool> {
		void decode(const std::string &value, bool &result) {
			std::string str(value);
			std::transform(str.begin(), str.end(), str.begin(), [](const char c) {return static_cast<char>(std::toupper(c)); }); 
			if (str == "TRUE")
				result = true;
			else if (str == "FALSE")
				result = false;
			else
				throw std::invalid_argument("field is not a bool");
		}
		void encode(const bool value, std::string &result) {
			// 这行代码，只要value给的true,result就会被赋予"true"，给的false，就会被赋予“false”
			result = value ? "true" : "false";
		}
	};

	template<>
	struct Convert<char> {
		void decode(const std::string &value, char &result) {
			assert(value.size() > 0);
			result = value[0];
		}
		void encode(const char value, std::string &result) {
			result = value;
		}
	};

	template<>
	struct Convert<unsigned char> {
		void decode(const std::string &value, unsigned char &result) {
			assert(value.size() > 0);
			result = value[0];
		}

		void encode(const unsigned char value, std::string &result) {
			result = value;
		}
	};

	template<>
	struct Convert<short> {
		void decode(const std::string &value, short &result) {
			long tmp;
			if (!strToLong(value, tmp))
				throw std::invalid_argument("field is not a short");
			result = static_cast<short>(tmp);
		}
		void encode(const short value, std::string &result) {
			/*  数字转字符串
			std::stringstream ss;
			ss << value;
			result = ss.str();
			*/
			// 简单的方法
			result = std::to_string(value);
		}
	};
	template<>
	struct Convert<unsigned short> {
		void decode(const std::string &value, unsigned short &result) {
			unsigned long tmp;
			if (!strToULong(value, tmp))
				throw std::invalid_argument("field is not an unsigned short");
			result = static_cast<unsigned short>(tmp);
		}

		void encode(const unsigned short value, std::string &result) {
			std::stringstream ss;
			ss << value;
			result = ss.str();
		}
	};
	template<>
	struct Convert<int> {
		void decode(const std::string &value, int &result) {
			long tmp;
			if (!strToLong(value, tmp))
				throw std::invalid_argument("field is not an int");
			result = static_cast<int>(tmp);
		}

		void encode(const int value, std::string &result) {
			std::stringstream ss;
			ss << value;
			result = ss.str();
		}
	};
	template<>
	struct Convert<unsigned int> {
		void decode(const std::string &value, unsigned int &result) {
			unsigned long tmp;
			if (!strToULong(value, tmp))
				throw std::invalid_argument("field is not an unsigned int");
			result = static_cast<unsigned int>(tmp);
		}

		void encode(const unsigned int value, std::string &result) {
			std::stringstream ss;
			ss << value;
			result = ss.str();
		}
	};
	template<>
	struct Convert<long> {
		void decode(const std::string &value, long &result) {
			if (!strToLong(value, result))
				throw std::invalid_argument("field is not a long");
		}

		void encode(const long value, std::string &result) {
			std::stringstream ss;
			ss << value;
			result = ss.str();
		}
	};
	template<>
	struct Convert<unsigned long> {
		void decode(const std::string &value, unsigned long &result) {
			if (!strToULong(value, result))
				throw std::invalid_argument("field is not an unsigned long");
		}

		void encode(const unsigned long value, std::string &result) {
			std::stringstream ss;
			ss << value;
			result = ss.str();
		}
	};
	template<>
	struct Convert<double> {
		void decode(const std::string &value, double &result) {
			result = std::stod(value);
		}

		void encode(const double value, std::string &result) {
			std::stringstream ss;
			ss << value;
			result = ss.str();
		}
	};
	template<>
	struct Convert<float> {
		void decode(const std::string &value, float &result) {
			result = std::stof(value);
		}

		void encode(const float value, std::string &result) {
			std::stringstream ss;
			ss << value;
			result = ss.str();
		}
	};

	template<>
	struct Convert<std::string> {
		void decode(const std::string &value, std::string &result) {
			result = value;
		}

		void encode(const std::string &value, std::string &result) {
			result = value;
		}
	};
	template<>
	struct Convert<const char*> {
		// 不太明白：const char* const &value  以及 const char* &result
		// 这里的 & 符号应该是代表引用的意思
		void encode(const char* const &value, std::string &result) {
			result = value;
		}
		void decode(const std::string &value, const char* &result) {
			result = value.c_str();
		}
	};
	template<>
	struct Convert<char*> {
		void encode(const char* const &value, std::string &result) {
			result = value;
		}
	};
	template<size_t n>
	struct Convert<char[n]> {
		void encode(const char *value, std::string &result) {
			result = value;
		}
	};
	
	
	class IniField {
	private:
		std::string value_;
	public:
		IniField() : value_() {}
		IniField(const std::string &value) : value_(value) {}
		IniField(const IniField &field) : value_(field.value_) {}
		~IniField() {}

		template<typename T>
		T as() const {
			Convert<T> conv;
			T result;
			conv.decode(this->value_, result);
			return result;
		}

		template<typename T>
		IniField &operator=(const T &value) {
			Convert<T> conv;
			conv.encode(value, this->value_);
			return *this;
		}

		IniField& operator=(const IniField &field) {
			this->value_ = field.value_;
			return *this;
		}
	};


	struct StringInsensitiveLess {
		bool operator()(std::string lhs, std::string rhs) const {
			std::transform(lhs.begin(), lhs.end(), lhs.begin(), 
				[](const char c) {return static_cast<char>(std::tolower(c)); });
			std::transform(rhs.begin(), rhs.end(), rhs.begin(),
				[](const char c) {return static_cast<char>(std::tolower(c)); });
			return lhs < rhs;
		}
	};
	

	template<typename Comparator>
	class IniSectionBase : public std::map<std::string, IniField, Comparator> {
	public:
		IniSectionBase() {}
		~IniSectionBase() {}
	};


	using IniSection = IniSectionBase<std::less<std::string>>;
	using IniSectionCaseInsensitive = IniSectionBase<StringInsensitiveLess>;



	/* 核心是这个类，它继承于一个map，而这个map的.second的类型又是一个继承于map的类；map的第三个参数不重要，这里的第三个参数也没产生多大的意义  */
	template<typename Comparator>
	class IniFileBase : public std::map<std::string, IniSectionBase<Comparator>, Comparator> {
	private:
		char fieldSep_ = '=';
		char esc_ = '\\';
		std::vector<std::string> commentPrefixes_ = { "#", ";" };  // 注释的开始
		bool multiLineValues_ = false;

		void eraseComment(const std::string &commentPrefixe, std::string &str, std::string::size_type startpos=0) {
			size_t prefixpos = str.find(commentPrefixe, startpos);
			if (prefixpos == std::string::npos) return;
			// Found a comment prefix, is it escaped?
			if (prefixpos != 0 && str.at(prefixpos - 1) == this->esc_) {
				// The comment prefix is escaped, so just delete the escape char
				// and keep erasing after the comment prefix
				str.erase(prefixpos - 1, 1);
				eraseComment(commentPrefixe, str, prefixpos - 1 + commentPrefixe.size());
			}
			else {
				str.erase(prefixpos);
			}
		}
		void eraseComments(std::string &str) {
			for (const std::string &commentPrefix : this->commentPrefixes_) {
				this->eraseComment(commentPrefix, str);
			}
		}

		/** Tries to find a suitable comment prefix for the string data at the given
		  * position. Returns commentPrefixes_.end() if not match was found. */
		std::vector<std::string>::const_iterator findCommentPrefix(const std::string &str, const std::size_t startpos) const {
			// if startpos is invalid simply return "not found"
			if (startpos >= str.size()) {
				return this->commentPrefixes_.end();
			}
				

			for (size_t i = 0; i < this->commentPrefixes_.size(); ++i) {
				const std::string &prefix = this->commentPrefixes_.at(i);
				// if this comment prefix is longer than the string view itself   then skip
				if (prefix.size() + startpos > str.size()) continue;

				bool match = true;
				for (size_t j = 0; j < prefix.size() && match; ++j) {
					match = str[startpos + j] == prefix[j];
				}
				if (match) return this->commentPrefixes_.begin() + i;
			}

			return this->commentPrefixes_.end();
		}

		void writeEscaped(std::ostream &os, const std::string &str) const {
			for (size_t i = 0; i < str.length(); ++i) {
				auto prefixpos = this->findCommentPrefix(str, i);
				// if no suitable prefix was found at this position
				// then simply write the current character
				if (prefixpos != this->commentPrefixes_.end()) {
					const std::string &prefix = *prefixpos;
					os.put(this->esc_);
					os.write(prefix.c_str(), prefix.size());
					i += prefix.size() - 1;
				}
				else if (this->multiLineValues_ && str[i] == '\n')
					os.write("\n\t", 2);
				else
					os.put(str[i]);
			}
		}

	public:
		IniFileBase() = default;
		IniFileBase(const char fileSep, const char comment) : fieldSep_(fileSep), commentPrefixes_(1, std::string(1, comment))  {}  // ???

		IniFileBase(const std::string &filename) {
			load(filename);
		}
		IniFileBase(std::istream is) {
			decode(is);
		}

		IniFileBase(const char fieldSep, const std::vector<std::string> &commentPrefixes)
			: fieldSep_(fieldSep), commentPrefixes_(commentPrefixes) { }

		IniFileBase(const std::string &filename, const char fieldSep, const std::vector<std::string> &commentPrefixes)
			: fieldSep_(fieldSep), commentPrefixes_(commentPrefixes) {
			load(filename);
		}

		IniFileBase(std::istream &is, const char fieldSep, const std::vector<std::string> &commentPrefixes) : fieldSep_(fieldSep), commentPrefixes_(commentPrefixes) {
			decode(is);   
		}

		~IniFileBase() {}

		/** Sets the separator charactor for fields in the INI file.
		  * @param sep separator character to be used. */
		void setFieldSep(const char sep) {
			this->fieldSep_ = sep;
		}

		/** Sets the character that should be interpreted as the start of comments.
		  * Default is '#'.
		  * Note: If the inifile contains the comment character as data it must be prefixed with
		  * the configured escape character.
		  * @param comment comment character to be used. */
		void setCommentChar(const char comment) {
			this->commentPrefixes_ = {std::string(1, comment)};
		}

		/** Sets the list of strings that should be interpreted as the start of comments.
		  * Default is [ "#" ].
		  * Note: If the inifile contains any comment string as data it must be prefixed with
		  * the configured escape character.
		  * @param commentPrefixes vector of comment prefix strings to be used. */
		void setCommentPrefixes(const std::vector<std::string> &commentPrefixes) {
			this->commentPrefixes_ = commentPrefixes;
		}

		/** 设置用于转义注释前缀的字符
		  * Default is '\'.
		  * @param esc escape character to be used. */
		void setEscapeChar(const char esc) {
			this->esc_ = esc;
		}

		/** Sets whether or not to parse multi-line field values.
		  * Default is false.
		  * @param enable enable or disable? */
		void setMultiLineValues(bool enable) {
			this->multiLineValues_ = enable;
		}

		/** Tries to decode a ini file from the given input stream.
		  * @param is input stream from which data should be read. */
		void decode(std::istream &is) {
			this->clear();     // 这个类继承的map容器，所以也是容器，先clear()一下，确保干净
			int lineNo = 0;
			IniSectionBase<Comparator> *currentSection = nullptr;
			std::string mutliLineValueFieldName = "";
			std::string line;
			// interate file line by line
			while (!is.eof() && !is.fail()) {
				std::getline(is, line, '\n');  // 第三个参数默认应该就是这，可以不给的
				this->eraseComments(line);
				bool hasIndent = line.find_first_not_of(indents()) != 0;
				trim(line);
				++lineNo;

				// skip if line is empty
				if (line.size() == 0) continue;

				if (line[0] == '[') {
					// line is a section
					// check if the section is also closed on same line
					std::size_t pos = line.find("]");
					if (pos == std::string::npos) {
						std::stringstream ss;
						ss << "1." << lineNo << ": ini parsing failed, section not closed";
						throw std::logic_error(ss.str());
					}
					// check if the section name is empty
					if (pos == 1) {
						std::stringstream ss;
						ss << "l." << lineNo << ": ini parsing failed, section is empty";
						throw std::logic_error(ss.str());
					}

					// retrieve section name  
					std::string secName = line.substr(1, pos - 1);
					// (*this)就是拿到类这个map,然后[secName]作为key，拿到对一个值(又是一个map装key:value)，然后&取地址
					currentSection = &((*this)[secName]);  // 注意这个写法

					// clear multiline value field name
					// a new section means there is no value to continue
					mutliLineValueFieldName = "";
				}
				else{
					// line is a field definition
					// check if section was already opened
					if (currentSection == nullptr) {
						std::stringstream ss;
						ss << "l." << lineNo << ": ini parsing failed, field has no section";
						throw std::logic_error(ss.str());
					}
					
					// find key value separator
					std::size_t pos = line.find(this->fieldSep_);
					if (multiLineValues_ && hasIndent && mutliLineValueFieldName != "") {
						// extend a multi-line value
						IniField previous_value = (*currentSection)[mutliLineValueFieldName]; // ??
						std::string value = previous_value.as<std::string>() + "\n" + line;
						(*currentSection)[mutliLineValueFieldName] = value;  
					}
					else if (pos == std::string::npos) {
						std::stringstream ss;
						ss << "l." << lineNo << ": ini parsing failed, no '"
							<< fieldSep_ << "' found";
						if (multiLineValues_)
							ss << ", and not a multi-line value continuation";
						throw std::logic_error(ss.str());
					}
					else {
						// retrieve field name and value
						std::string name = line.substr(0, pos);
						trim(name);
						std::string value = line.substr(pos + 1, std::string::npos);
						trim(value);

						// 这就是map的插值写法；跟下面那行一个意思
						(*currentSection)[name] = value;   
						// (*currentSection).insert(std::make_pair(name, value));

						// store last field name for potential multi-line values
						mutliLineValueFieldName = name;
					}
				
				}
			}
		}

		/** Tries to decode a ini file from the given input string.
		 * @param content string to be decoded. */
		void decode(const std::string &content) {
			std::istringstream ss(content);
			decode(ss);
		}

		/** Tries to load and decode a ini file from the file at the given path.
		  * @param fileName path to the file that should be loaded. */
		void load(const std::string &fileName) {
			std::ifstream is(fileName.c_str());
			decode(is);
		}

		/** Encodes this inifile object and writes the output to the given stream.
		  * @param os target stream. */
		void encode(std::ostream &os) const {
			// iterate through all sections in this file
			// 这个类继承了map，本身也就成为了map，可以去读数据
			for (const auto &filePair : *this) {      // ？?
				os.put('[');
				this->writeEscaped(os, filePair.first);  // first是section
				os.put(']').put('\n');

				// iterate through all fields in the section
				for (const auto &secPair : filePair.second) {  // section里面包裹这key:value
					this->writeEscaped(os, secPair.first);
					os.put(this->fieldSep_);
					this->writeEscaped(os, secPair.second.template as<std::string>());  // ??
					os.put('\n');
				}
			}
		}

		/** Encodes this inifile object as string and returns the result.
		  * @return encoded infile string. */
		std::string encode() const {
			std::ostringstream ss;;
			this->encode(ss);
			return ss.str();
		}

		/** Saves this inifile object to the file at the given path.
		  * @param fileName path to the file where the data should be stored. */
		void save(const std::string &fileName) const {
			std::ofstream os(fileName.c_str());
			this->encode(os);
		}
	};


	using IniFile = IniFileBase<std::less<std::string>>;
	using IniSection = IniSectionBase<std::less<std::string>>;
	using IniFileCaseInsensitive = IniFileBase<StringInsensitiveLess>;
	using IniSectionCaseInsensitive = IniSectionBase<StringInsensitiveLess>;
	

}   // namespace ini

#endif // !INICPP_H
