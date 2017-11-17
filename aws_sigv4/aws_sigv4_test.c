#include "aws_sigv4/minunit.h"
#include "aws_sigv4/aws_status.h"
#include "aws_sigv4/aws_sigv4.h"
#include "aws_sigv4/aws.h"
static aws_t context = NULL;

static char *test_create() {
	context = aws_init("us-east-1", "glacier", "glacier.aws.amazon.com", "/",
			"GET");
	mu_assert(context != NULL, "aws_init failed");

	return NULL;
}

static char *test_add_bad_headers() {
	int status;
	status = aws_add_header(context, "", "hello");
	mu_assert(status == AWS_ERR,
			"Adding header with empty key should not be accepted");

	status = aws_add_header(context, "hello", "");
	mu_assert(status == AWS_ERR,
			"Adding header with empty value should not be accepted");

	status = aws_add_header(context, "", "");
	mu_assert(status == AWS_ERR,
			"Adding header with empty key and value should not be accepted");

	return NULL;
}

static char *test_add_bad_params() {
	int status;
	status = aws_add_param(context, "", "hello");
	mu_assert(status == AWS_ERR,
			"Adding param with empty key should not be accepted");

	status = aws_add_param(context, "hello", "");
	mu_assert(status == AWS_ERR,
			"Adding param with empty value should not be accepted");

	status = aws_add_param(context, "", "");
	mu_assert(status == AWS_ERR,
			"Adding param with empty key and value should not be accepted");

	return NULL;
}

static char *test_add_params() {
	aws_add_param(context, "key4", "val4");
	aws_add_param(context, "key3", "val3");
	aws_add_param(context, "Key2", "val2");
	aws_add_param(context, "key1", "val1");

	/* TODO */

	return NULL;
}

static char *test_add_headers() {
	aws_add_header(context, "key4", "val4");
	aws_add_header(context, "key3", "val3");
	aws_add_header(context, "Key2", "val2");
	aws_add_header(context, "key1", "val1");

	/* TODO */

	return NULL;
}

static char *test_sign() {
	char *date = "Mon, 09 Sep 2011 23:36:00 GMT";
	char signature[65];
	int status;

	status = aws_sign(context, "wJalrXUtnFEMI/K7MDENG+bPxRfiCYEXAMPLEKEY", date,
			signature);
	mu_assert(status == AWS_OK, "aws_sign failed");

	return NULL;
}

static char *test_example() {
	char *date = "Mon, 09 Sep 2011 23:36:00 GMT";
	char *secret = "wJalrXUtnFEMI/K7MDENG+bPxRfiCYEXAMPLEKEY";
	char signature[65];

	aws_cleanup(context);
	context = aws_init("us-east-1", "host", "host.foo.com", "/", "GET");

	aws_sign(context, secret, date, signature);

	mu_assert(
			strcmp(signature,
					"b27ccfbfa7df52a200ff74193ca6e32d4b48b8856fab7ebf1c595d0670a7e470")
					== 0, "Signatures don't match");
	if (!strcmp(signature,
			"b27ccfbfa7df52a200ff74193ca6e32d4b48b8856fab7ebf1c595d0670a7e470")) {
		printf("test_example true \r\n");
	} else {
		printf("test_example false \r\n");

	}

	return NULL;
}

static char *test_add_bad_http_method() {
	aws_cleanup(context);
	context = aws_init("us-east-1", "host", "host.foo.com", "/", "GAT");

	mu_assert(context == NULL,
			"Invalid http method should return a null context");

	aws_cleanup(context);
	context = aws_init("us-east-1", "host", "host.foo.com", "/", "");

	mu_assert(context == NULL, "Empty http method should return a null context");

	return NULL;
}

static char *test_cleanup() {
	aws_cleanup(context);

	return NULL;
}

/****************************************************************/
static char *my_test_create() {
	context = aws_init("eu-central-1", "iotdata",
			"aujd0l630ek9p.iot.eu-central-1.amazonaws.com", "/", "GET");
	mu_assert(context != NULL, "aws_init failed");

	return NULL;
}
char *str =
		"content-type:application/x-amz-json-1.0\nhost:aujd0l630ek9p.iot.eu-central-1.amazonaws.com\nx-amz-date:20171116T094507Z\n";
static char *my_test_add_headers() {
	aws_add_header(context, "Http_Method", "GET");
	aws_add_header(context, "Shadow_URI", "/things/RaspberryPi/shadow");
//	aws_add_header(context, "Http_Query_String", "");
	aws_add_header(context, "key1", str);
	aws_add_header(context, "Http_Request_Signed_Headers",
			"content-type;host;x-amz-date");
	/* TODO */

	return NULL;
}

static char *date_test_sign() {
//	char *date = "20171116T064149Z";
	char *date = "Thu, 16 Nov 2017 08:42:42 GMT";
	char signature[65];
	int status;

	status = aws_sign(context, "wJalrXUtnFEMI/K7MDENG+bPxRfiCYEXAMPLEKEY", date,
			signature);
	mu_assert(status == AWS_OK, "aws_sign failed");

	return NULL;
}

static char *my_test_add_params() {
	aws_add_param(context, "Http_Method", "GET");
	aws_add_param(context, "Shadow_URI", "/things/RaspberryPi/shadow");
//	aws_add_header(context, "Http_Query_String", "");
	aws_add_param(context, "key1", str);
	aws_add_param(context, "Http_Request_Signed_Headers",
			"content-type;host;x-amz-date");
	/* TODO */

	return NULL;
}

char *aws_sigv4_test() {
	char *date = "Thu, 17 Nov 2017 07:06:35 GMT";
	char *secret = "Cc7DU8ij+COJn1xmQ3Z0GIf+aXICYxrpN5gUsW3R";
	char signature[65];

	aws_cleanup(context);
	context = aws_init("eu-central-1", "iotdata",
			"aujd0l630ek9p.iot.eu-central-1.amazonaws.com", "/things/RaspberryPi/shadow", "GET");



	aws_add_header(context, "content-type", "application/x-amz-json-1.0");
//	aws_add_header(context, "Shadow_URI", "/things/RaspberryPi/shadow");
//	//	aws_add_header(context, "Http_Query_String", "");
//	aws_add_header(context, "key1", str);
//	aws_add_header(context, "Http_Request_Signed_Headers",
//			"content-type;host;x-amz-date");

	aws_sign(context, secret, date, signature);
	printf("\r\nsignature=%s\r\n", signature);
	mu_assert(
			strcmp(signature,
					"1fb3e468e079cd953b58c6cda268c75aeababf256daf81e0bfbf4b0d25869ac2")
					== 0, "Signatures don't match");
	if (!strcmp(signature,
			"1fb3e468e079cd953b58c6cda268c75aeababf256daf81e0bfbf4b0d25869ac2")) {
		printf("test_example true \r\n");
	} else {
		printf("test_example false \r\n");

	}

	return NULL;
}
//RUN_TESTS(all_tests);

char *aws_sigv4_all_tests() {
	mu_suite_start();
//	mu_run_test(my_test_create);
//	mu_run_test(my_test_add_params);
	mu_run_test(aws_sigv4_test)
//	mu_run_test(test_add_bad_headers);
//	mu_run_test(test_add_headers);
//	mu_run_test(test_add_bad_params);
//	mu_run_test(test_add_params);
//	mu_run_test(test_sign);
//	mu_run_test(test_example);
//	mu_run_test(test_add_bad_http_method);
//	mu_run_test(test_cleanup);

	return NULL;
}
