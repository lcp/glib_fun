#include <stdio.h>
#include <string.h>
#include <glib.h>

/**
 * http://www.zytrax.com/tech/survival/ssl.html
 *
 * # shown split across two lines for presentation reasons only
 * C=MY,ST=another state,L=another city,O=my company,OU=certs, CN=www.example.com
 * # various interpretations of the RDN fields exist
 * # the following are presented as generally accepted
 * # values. In the case of personal certificates GN=, SN= or pseudonym=
 * # can appear in the fields
 * # C = ISO3166 two character country code
 * # ST = state or province
 * # L = Locality; generally means city
 * # O = Organization - Company Name
 * # OU = Organization Unit - division or unit
 * # CN = CommonName - entity name e.g. www.example.com
 */

#define SUBJECT_1 "/C=US/ST=Utah/L=Provo/O=Whatever, Inc./CN=nobody.there.nosuchcompany.com"
#define SUBJECT_2 "/C=TW/ST=Taiwan/L=Taipei/O=Nobody/OU=NoOne/CN=WhoTheHellAmI/emailAddress=who@nobody.com"

struct subject
{
	char *country;
	char *state;
	char *locality;
	char *org;
	char *org_unit;
	char *common_name;
	char *email;
};

guint
count_variable_length (const char *input)
{
	char *ptr;
	guint length = 0;

	g_return_val_if_fail (input != NULL, 0);

	ptr = (char *)input;
	while (*ptr != '/' && *ptr != '\0') {
		length++;
		ptr++;
	}

	return length;
}

struct subject *
parse_subject (const char *subject_str)
{
	struct subject *subject;
	char *ptr;
	guint length = 0;

	g_return_val_if_fail (subject_str != NULL, NULL);

	subject = g_new0 (struct subject, 1);

	ptr = (char *)subject_str;
	while (*ptr != '\0') {
		if (strncmp (ptr, "/C=", 3) == 0) {
			ptr += 3;
			length = count_variable_length(ptr);
			subject->country = g_strndup (ptr, length);
		} else if (strncmp (ptr, "/ST=", 4) == 0) {
			ptr += 4;
			length = count_variable_length(ptr);
			subject->state = g_strndup (ptr, length);
		} else if (strncmp (ptr, "/L=", 3) == 0) {
			ptr += 3;
			length = count_variable_length(ptr);
			subject->locality = g_strndup (ptr, length);
		} else if (strncmp (ptr, "/O=", 3) == 0) {
			ptr += 3;
			length = count_variable_length(ptr);
			subject->org = g_strndup (ptr, length);
		} else if (strncmp (ptr, "/OU=", 4) == 0) {
			ptr += 4;
			length = count_variable_length(ptr);
			subject->org_unit = g_strndup (ptr, length);
		} else if (strncmp (ptr, "/CN=", 4) == 0) {
			ptr += 4;
			length = count_variable_length(ptr);
			subject->common_name = g_strndup (ptr, length);
		} else if (strncmp (ptr, "/emailAddress=", 14) == 0) {
			ptr += 14;
			length = count_variable_length(ptr);
			subject->email = g_strndup (ptr, length);
		} else {
			length = 1;
		}

		ptr += length;
	}

	return subject;
}

void
free_subject (struct subject *subject)
{
	g_return_if_fail (subject != NULL);

	g_free (subject->country);
	g_free (subject->state);
	g_free (subject->locality);
	g_free (subject->org);
	g_free (subject->org_unit);
	g_free (subject->common_name);
	g_free (subject->email);

	g_free (subject);
}

void
print_subject (struct subject *subject)
{
	if (subject->country)
		printf ("Country = %s\n", subject->country);
	if (subject->state)
		printf ("State = %s\n", subject->state);
	if (subject->locality)
		printf ("Locality = %s\n", subject->locality);
	if (subject->org)
		printf ("Organization = %s\n", subject->org);
	if (subject->org_unit)
		printf ("Organization Unit = %s\n", subject->org_unit);
	if (subject->common_name)
		printf ("Common Name = %s\n", subject->common_name);
	if (subject->email)
		printf ("Email = %s\n", subject->email);
}

int
main()
{
	struct subject *subject;

	subject = parse_subject (SUBJECT_1);

	print_subject (subject);

	free_subject (subject);

	printf ("\n====\n\n");

	subject = parse_subject (SUBJECT_2);

	print_subject (subject);

	free_subject (subject);

	return 0;
}
