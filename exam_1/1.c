/************************************
 * 调用libxml2库解析xml，提取出电话薄信息
 *
 * @author:Anker  @date:2014/02/09
 * *********************************/

#include <stdio.h>
#include <assert.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#define DEFAULT_XML_FILE "/work/study/xml/exam_1/phone_book.xml"

//解析每一个phone，提取出name、tel、address
static int parse_phone(xmlDocPtr doc, xmlNodePtr cur)
{
	assert(doc || cur);
	xmlChar *key;

	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		//获取name
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"name"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("name: %s\t", key);
			xmlFree(key);
		}
		//获取tel
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"tel"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("tel: %s\t", key);
			xmlFree(key);
		}
		//获取address
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"address"))) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("address: %s\n", key);
			xmlFree(key);
		}
		cur = cur->next;
	}
	return 0;
}

static int parse_phone_book(const char *file_name)
{
	assert(file_name);

	xmlDocPtr doc;   //xml整个文档的树形结构
	xmlNodePtr cur;  //xml节点
	xmlChar *id;     //phone id

	//获取树形结构
	doc = xmlParseFile(file_name);
	if (doc == NULL) {
		fprintf(stderr, "Failed to parse xml file:%s\n", file_name);
		goto FAILED;
	}

	//获取根节点
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		fprintf(stderr, "Root is empty.\n");
		goto FAILED;
	}

	if ((xmlStrcmp(cur->name, (const xmlChar *)"phone_books"))) {
		fprintf(stderr, "The root is not phone_books.\n");
		goto FAILED;
	}

	//遍历处理根节点的每一个子节点
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {
		if ((!xmlStrcmp(cur->name, (const xmlChar *)"phone"))) {
			id = xmlGetProp(cur, "id");
			printf("id:%s\t",id);
			parse_phone(doc, cur);
		}
		cur = cur->next;
	}
	xmlFreeDoc(doc);
	return 0;
FAILED:
	if (doc) {
		xmlFreeDoc(doc);
	}
	return -1;
}

int main(int argc, char*argv[])
{
	char *xml_file = DEFAULT_XML_FILE;

	if (argc == 2) {
		xml_file = argv[1];
	}

	if (parse_phone_book(xml_file) != 0) {
		fprintf(stderr, "Failed to parse phone book.\n");
		return -1;
	}

	return 0;
}
