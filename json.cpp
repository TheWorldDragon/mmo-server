#include "all.hpp"
json_t::json_t(const char* json_string) noexcept
{
	this->json_string = strdup(json_string);
}
json_t::~json_t() noexcept
{
	free(json_string);
}
char* json_t::get_string() noexcept
{
	return strdup(json_string);
}
