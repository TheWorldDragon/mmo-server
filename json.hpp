#ifndef JSON_HPP
#define JSON_HPP

class json_t
{
	char* json_string;
	public:
		json_t(const char* json_string) noexcept;
		~json_t() noexcept;
		char* get_string() noexcept;


};
#endif
