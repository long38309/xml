/****************************************
 *练习libxml库，创建通讯录xml文档，新增一个通讯录，
 *如果xml文件存在，则添加一个新节点
 *如果xml文件不存在，则新建一个xml文件
 *
 *@author: Anker  @date: 2014/02/09
 * ***************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>

#define PHONE_BOOK_FILE     "/work/study/xml/exam_1/phone_book.xml"
#define ID_STR_LEN        16
#define NAME_STR_LEN         32
#define TEL_STR_LEN         16
#define ADDR_STR_LEN         128

//电话通讯录结构体
typedef struct phone_t {
	int id;              //编号
	char name[NAME_STR_LEN];     //姓名
	char tel[TEL_STR_LEN];       //电话
	char address[ADDR_STR_LEN];  //地址
}phone;

//设置通讯录项
static void set_phone_item(phone *phone_item)
{
	assert(phone_item);

	phone_item->id = 10;
	snprintf(phone_item->name, NAME_STR_LEN, "%s", "Anker");
	snprintf(phone_item->tel, TEL_STR_LEN, "%s", "13223246599");
	snprintf(phone_item->address, ADDR_STR_LEN, "%s", "Shenzheng");
}

//创建phone节点
static xmlNodePtr create_phone_node(const phone *phone_item)
{
	assert(phone_item);

	char id[ID_STR_LEN] = {0};
	xmlNodePtr phone_node = NULL;

	phone_node = xmlNewNode(NULL, BAD_CAST"phone");
	if (phone_node == NULL) {
		fprintf(stderr, "Failed to create new node.\n");
		return NULL;
	}
	//设置属性
	snprintf(id, ID_STR_LEN, "%d", phone_item->id);
	xmlNewProp(phone_node, BAD_CAST"id", (xmlChar*)id);

	xmlNewChild(phone_node, NULL, BAD_CAST"name", (xmlChar *)phone_item->name);
	xmlNewChild(phone_node, NULL, BAD_CAST"tel", (xmlChar *)phone_item->tel);
	xmlNewChild(phone_node, NULL, BAD_CAST"address", (xmlChar *)phone_item->address);

	return phone_node;
}

//向根节点中添加一个phone节点
static int add_phone_node_to_root(xmlNodePtr root_node)
{
	xmlNodePtr phone_node = NULL;
	phone *phone_item = NULL;

	//创建一个新的phone
	phone_item = (phone *)malloc(sizeof(phone));
	if (phone_item == NULL) {
		fprintf(stderr, "Failed to malloc memory.\n");
		return -1;
	}
	set_phone_item(phone_item);

	//创建一个phone节点
	phone_node = create_phone_node(phone_item); 
	if (phone_node == NULL) {
		fprintf(stderr, "Failed to create phone node.\n");
		goto FAILED;
	}
	//根节点添加一个子节点
	xmlAddChild(root_node, phone_node);
	free(phone_item);

	return 0;
FAILED:
	if (phone_item){
		free(phone_item);
	}
	return -1;
}

//创建phone_books
static int create_phone_books(const char *phone_book_file)
{
	assert(phone_book_file);

	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL;

	//创建一个xml 文档
	doc = xmlNewDoc(BAD_CAST"1.0");
	if (doc == NULL) {
		fprintf(stderr, "Failed to new doc.\n");
		return -1;
	}

	//创建根节点
	root_node = xmlNewNode(NULL, BAD_CAST"phone_books");
	if (root_node == NULL) {
		fprintf(stderr, "Failed to new root node.\n");
		goto FAILED;
	}
	//将根节点添加到文档中
	xmlDocSetRootElement(doc, root_node);

	if (add_phone_node_to_root(root_node) != 0) {
		fprintf(stderr, "Failed to add a new phone node.\n");
		goto FAILED;
	}
	//将文档保存到文件中，按照utf-8编码格式保存
	xmlSaveFormatFileEnc(phone_book_file, doc, "UTF-8", 1);
	//xmlSaveFile("test.xml", doc);
	xmlFreeDoc(doc);

	return 0; 
FAILED:
	if (doc) {
		xmlFreeDoc(doc);
	}

	return -1;
}

static int add_phone_node(const char *phone_book_file)
{
	assert(phone_book_file);

	xmlDocPtr doc = NULL;
	xmlNodePtr root_node = NULL;
	xmlNodePtr phone_node = NULL;
	phone *phone_item = NULL;

	doc = xmlParseFile(phone_book_file);
	if (doc == NULL) {
		fprintf(stderr, "Failed to parser xml file:%s\n", phone_book_file);
		return -1;
	}

	root_node = xmlDocGetRootElement(doc);
	if (root_node == NULL) {
		fprintf(stderr, "Failed to get root node.\n");
		goto FAILED;
	}

	if (add_phone_node_to_root(root_node) != 0) {
		fprintf(stderr, "Failed to add a new phone node.\n");
		goto FAILED;
	}
	//将文档保存到文件中，按照utf-8编码格式保存
	xmlSaveFormatFileEnc(phone_book_file, doc, "UTF-8", 1);
	xmlFreeDoc(doc);

	return 0;
FAILED:
	if (doc) {
		xmlFreeDoc(doc);
	}

	return -1;
}

int main(int argc, char *argv[])
{
	char *phone_book_file = PHONE_BOOK_FILE;

	if (argc == 2) {
		phone_book_file = argv[1];
	}

	if (access(phone_book_file, F_OK) == 0) {
		//文件存在，添加一个新的phone节点
		add_phone_node(phone_book_file);
	}
	else {
		//文件不存在，创建一个信息的phone book
		create_phone_books(phone_book_file);
	}

	return 0;
}
