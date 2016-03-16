#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <assert.h>
#define ASSERT assert

#define CONF_FILE	"/work/study/xml/action_sets.xml"

// add a dev to action_group
//		glcil adddev default_group 54:27:1e:6c:04:df 

//	del a dev from action_group
//		glcil deldev default_group 54:27:1e:6c:04:df 

// add a action entry to action_group
// 	

// del a action entry from action_group
// 	

// glcli listdomain default_group

// add a action entry to action_group
// 		glcil addact default_group domain action redirect

// del all action entry to action_group
// 		glcil delact default_group 



#define CMD_NONE		0x00
#define CMD_ADDDEV		0x01
#define CMD_DELDEV		0x02
#define CMD_LISTDOMAIN	0x03
#define CMD_ADDACT		0x04
#define CMD_DELACT		0x05


///////////////////////////////////////////////////////////////////////////////////////
static char *get_node_attrib(xmlNode *node, const char *key)
{
	xmlChar *attrib;

	attrib = xmlGetProp(node, (const xmlChar *)key);
	return (char *)attrib;
}

static int set_node_attrib(xmlNode *node, const char *key, const char *val)
{
	xmlSetProp(node, (const xmlChar *)key, (const xmlChar *)val);
	return 0;
}

static void put_node_attrib(char *attrib)
{
	ASSERT(attrib);
	xmlFree((xmlChar *)attrib);
}

static char *get_node_cnt(xmlNode *node, const char *key)
{
	xmlChar *cnt;
	xmlNode *curr;

	curr = node->children;
	while (curr != NULL) {
		if (!xmlStrcmp(curr->name, (const xmlChar *)key)) {
			cnt = xmlNodeListGetString(node->doc, curr->children, 1);
			return (char *)cnt;
		}
		curr = curr->next;
	}
	return NULL;
}

static int set_node_cnt(xmlNode *node, const char *key, const char *val)
{
	xmlNode *curr;

	curr = node->children;
	while (curr != NULL) {
		if (!xmlStrcmp(curr->name, (const xmlChar *)key)) {
			xmlNodeSetContent(curr, (const xmlChar *)val);
			return 0;
		}
		curr = curr->next;
	}
	return -1;
}

static void put_node_cnt(char *cnt)
{
	ASSERT(cnt);
	xmlFree((xmlChar *)cnt);
}

static xmlNode *find_node(xmlNode *parent, const char *name)
{
	xmlNode *node;

	for (node = parent->children; node; node = node->next) {
		if ((node->type == XML_ELEMENT_NODE) &&
				!xmlStrcmp(node->name, (const xmlChar *)name))
			return node;
	}
	return NULL;
}
//////////////////////////////////////////////////////////////////////////////////


static int add_dev(int argc, char *argv[])
{
	xmlDoc *doc;
	xmlNode *root;
	char *hwaddr;
	xmlNode *curr;
	char *group;

	if (argc != 4) {
		return -1;
	}

	group = argv[2];
	hwaddr = argv[3];

	doc = xmlReadFile(CONF_FILE, "UTF-8", XML_PARSE_RECOVER);
	if (doc == NULL) {
		return -1;
	}
	root = xmlDocGetRootElement(doc);
	if (root == NULL) {
		xmlFreeDoc(doc);
		xmlCleanupParser();
		return -2;
	}

	curr = root->children;
	for (; curr; curr = curr->next) {
		if (!strcmp((const char *)curr->name, "action_group")) {
			char *p;
			if ((p = get_node_cnt(curr, "name")) != NULL) {
				if (strcmp(p, group) == 0) {
					break;
				}
			}
		}
	}

	/* add new dev to this group */
	if (curr != NULL) {
		xmlNewTextChild (curr, NULL, BAD_CAST "mac_entry", hwaddr);
	}

	xmlSaveFormatFileEnc(CONF_FILE, doc,"UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();

	return 0;
}

static int del_dev(int argc, char *argv[])
{
	xmlDoc *doc;
	xmlNode *root;
	char *hwaddr;
	xmlNode *node;
	xmlNode *curr;
	char *group;

	if (argc != 4) {
		return -1;
	}

	group = argv[2];
	hwaddr = argv[3];

	doc = xmlReadFile(CONF_FILE, "UTF-8", XML_PARSE_RECOVER);
	if (doc == NULL) {
		return -1;
	}
	root = xmlDocGetRootElement(doc);
	if (root == NULL) {
		xmlFreeDoc(doc);
		xmlCleanupParser();
		return -2;
	}

	node = root->children;
	for (; node; node = node->next) {
		if (!strcmp((const char *)node->name, "action_group")) {
			char *p;
			if ((p = get_node_cnt(node, "name")) != NULL) {
				if (strcmp(p, group) == 0) {
					break;
				}
			}
		}
	}

	/* del the dev from this group */
	if (node != NULL) {
		curr = node->children;
		for (; curr; curr = curr->next) {
			if (!strcmp((const char *)curr->name, "mac_entry")) {
				char *p;
				p = xmlNodeListGetString(node->doc, curr->children, 1);
				if (p != NULL) {
					if (strcmp(p, hwaddr) == 0) {
						break;
					}
				}
			}
		}

		if (curr != NULL) {
			xmlUnlinkNode(curr);
			xmlFreeNode(curr);
		}
	}

	xmlSaveFormatFileEnc(CONF_FILE, doc,"UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();

	return 0;
}

static int list_domain(int argc, char *argv[])
{
	xmlDoc *doc;
	xmlNode *root;
	xmlNode *node;
	xmlNode *curr;
	char *group;
	char *age;

	if (argc != 3) {
		return -1;
	}

	group = argv[2];

	doc = xmlReadFile(CONF_FILE, "UTF-8", XML_PARSE_RECOVER);
	if (doc == NULL) {
		return -1;
	}
	root = xmlDocGetRootElement(doc);
	if (root == NULL) {
		xmlFreeDoc(doc);
		xmlCleanupParser();
		return -2;
	}

	node = root->children;                 //获取根节点,打印根节点的话，值为text。
	for (; node; node = node->next) {      //遍历子节点，找出节点名为action_group		
		if (!strcmp((const char *)node->name, "action_group")) {
			char *p;
			if ((p = get_node_cnt(node, "name")) != NULL) {  //用节点名来获取节点的值
				if (strcmp(p, group) == 0) {
					break;
				}
				put_node_cnt(p);
			}
		}
	}


	/* check the dev from this group */
	if (node != NULL)                      //找到对应的action_group 节点后，输出想要子节点里的值
	{
		curr = node->children;
		for (; curr; curr = curr->next) 
		{
			//printf("%s\n", (const char *)curr->name);
			if (!strcmp((const char *)curr->name, "action_entry")) 
			{	
				char *p;
				p = get_node_cnt(curr, "domain");
				if (p != NULL) 
				{
					printf("%s\n", p);
					put_node_cnt(p);
				}
			}
		}
	}

	xmlFreeDoc(doc);
	xmlCleanupParser();

	return 0;
}

static int add_act(int argc, char *argv[])
{
	xmlDoc *doc;
	xmlNode *root;
	char *domain;
	char *action;
	char *redirect;
	xmlNode *curr;
	xmlNode *node;
	char *group;

	if (argc < 5) {
		return -1;
	}

	group = argv[2];
	domain = argv[3];
	action = argv[4];
	redirect = NULL;
	if (argc > 5) {
		redirect = argv[5];
	}

	doc = xmlReadFile(CONF_FILE, "UTF-8", XML_PARSE_RECOVER);
	if (doc == NULL) {
		return -1;
	}
	root = xmlDocGetRootElement(doc);
	if (root == NULL) {
		xmlFreeDoc(doc);
		xmlCleanupParser();
		return -2;
	}

	curr = root->children;
	for (; curr; curr = curr->next) {
		if (!strcmp((const char *)curr->name, "action_group")) {
			char *p;
			if ((p = get_node_cnt(curr, "name")) != NULL) {
				if (strcmp(p, group) == 0) {
					break;
				}
				put_node_cnt(p);
			}
		}
	}

	/* add new act to this group */
	if (curr != NULL) {
		node = xmlNewNode(NULL, BAD_CAST "action_entry");
		if (node != NULL) {
			xmlAddChild(curr, node);
			xmlNewTextChild(node, NULL, BAD_CAST "domain", domain);
			xmlNewTextChild(node, NULL, BAD_CAST "action", action);
			if (redirect != NULL)
				xmlNewTextChild(node, NULL, BAD_CAST "redirect_domain", redirect);
		}
	}

	xmlSaveFormatFileEnc(CONF_FILE, doc, "UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();

	return 0;
}

static int del_act(int argc, char *argv[])
{
	xmlDoc *doc;
	xmlNode *root;
	xmlNode *node;
	xmlNode *curr, *curr1;
	char *group;
	char *age;

	if (argc != 3) {
		return -1;
	}

	group = argv[2];

	doc = xmlReadFile(CONF_FILE, "UTF-8", XML_PARSE_RECOVER);
	if (doc == NULL) {
		return -1;
	}
	root = xmlDocGetRootElement(doc);
	if (root == NULL) {
		xmlFreeDoc(doc);
		xmlCleanupParser();
		return -2;
	}

	node = root->children;                 //获取根节点,打印根节点的话，值为text。
	for (; node; node = node->next) {      //遍历子节点，找出节点名为action_group		
		if (!strcmp((const char *)node->name, "action_group")) {
			char *p;
			if ((p = get_node_cnt(node, "name")) != NULL) {  //用节点名来获取节点的值
				if (strcmp(p, group) == 0) {
					break;
				}
				put_node_cnt(p);
			}
		}
	}

	if (node != NULL)   //找到对应的action_group 节点后，输出想要子节点里的值
	{		
		curr = node->children;

		while(curr)
		{
			curr1 = curr->next;
			if (!strcmp((const char *)curr->name, "action_entry")) 
			{
				xmlUnlinkNode(curr);
				xmlFreeNode(curr);
			}
			curr = curr1;
		}
	}

	xmlSaveFormatFileEnc(CONF_FILE, doc, "UTF-8", 1);	
	xmlFreeDoc(doc);
	xmlCleanupParser();

	return 0;

}


int main(int argc, char *argv[])
{
	int cmd = 0;

	/* get the cmd */
	cmd = CMD_NONE;
	if (argc < 2) {
		return -1;
	}
	if (strcmp(argv[1], "adddev") == 0) {
		cmd = CMD_ADDDEV;
	}
	if (strcmp(argv[1], "deldev") == 0) {
		cmd = CMD_DELDEV;
	}

	if (strcmp(argv[1], "listdomain") == 0) {
		cmd = CMD_LISTDOMAIN;
	}

	if (strcmp(argv[1], "addact") == 0) {
		cmd = CMD_ADDACT;
	}

	if (strcmp(argv[1], "delact") == 0) {
		cmd = CMD_DELACT;
	}

	if (cmd == CMD_NONE) {
		return -2;
	}

	switch (cmd) {
		case CMD_ADDDEV:
			add_dev(argc, argv);
			break;
		case CMD_DELDEV:
			del_dev(argc, argv);
			break;

		case CMD_LISTDOMAIN:
			list_domain(argc, argv);
			break;

		case CMD_ADDACT:
			add_act(argc, argv);
			break;

		case CMD_DELACT:
			del_act(argc, argv);
			break;

		default:
			break;
	}

	return 0;
}
