#include "all.h"
struct json
{
	char* str;
};
json_t* json_new(const char* json_string)
{
	json_t* self = malloc(sizeof(json_t));
	self->str = strdup(json_string);
	return self;
}
void json_delete(json_t* self)
{
	free(self->str);
	free(self);
}
char* json_string(json_t*self)
{
	return strdup(self->str);
}
#if 0
char* json_get(const char* key)
{
}
void json_delete_get(char*value)
{
}
void json_set(const char* key, const char* value)
{
	assert(false);
}
#endif
