#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xmlwriter.h>
#include <stdio.h>
#include <string.h>


int main()
{
	xmlDocPtr doc;
	xmlTextWriterPtr writer;
	int ret;

	writer = xmlNewTextWriterDoc(&doc, 0);
	ret = xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);

	ret = xmlTextWriterStartElement(writer, BAD_CAST "action_sets");

	ret = xmlTextWriterStartElement(writer, BAD_CAST "action_group");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "name", "%s", "5-8");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "describe", "%s", "default group actions");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "default_action", "%s", "accept");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "mac_entry", "%s", "aa:aa:aa:aa:aa:aa");
	ret = xmlTextWriterStartElement(writer, BAD_CAST "action_entry");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "domain", "%s", "www.baidu.com");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "action", "%s", "reject");
	ret = xmlTextWriterEndElement(writer);
	ret = xmlTextWriterEndElement(writer);

	ret = xmlTextWriterStartElement(writer, BAD_CAST "action_group");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "name", "%s", "8-10");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "describe", "%s", "default group actions");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "default_action", "%s", "accept");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "mac_entry", "%s", "aa:aa:aa:aa:aa:aa");
	ret = xmlTextWriterStartElement(writer, BAD_CAST "action_entry");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "domain", "%s", "www.baidu.com");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "action", "%s", "reject");
	ret = xmlTextWriterEndElement(writer);
	ret = xmlTextWriterEndElement(writer);

	ret = xmlTextWriterStartElement(writer, BAD_CAST "action_group");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "name", "%s", "10-18");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "describe", "%s", "default group actions");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "default_action", "%s", "accept");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "mac_entry", "%s", "aa:aa:aa:aa:aa:aa");
	ret = xmlTextWriterStartElement(writer, BAD_CAST "action_entry");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "domain", "%s", "www.baidu.com");
	ret = xmlTextWriterWriteFormatElement(writer, BAD_CAST "action", "%s", "reject");
	ret = xmlTextWriterEndElement(writer);
	ret = xmlTextWriterEndElement(writer);


	ret = xmlTextWriterEndElement(writer);

	ret = xmlTextWriterEndDocument(writer);

	xmlFreeTextWriter(writer);
	xmlSaveFormatFileEnc("./cdl.xml", doc, "UTF-8", 1);
	xmlFreeDoc(doc);


	return 0;
}
