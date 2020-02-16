<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" doctype-system="test.dtd"/>

  <xsl:template match="/">
    <HTML>
    <xsl:element name="HEAD">
      <xsl:element name="TITLE">Test</xsl:element>
    </xsl:element>
    <xsl:call-template name="name1"/>
    </HTML>
  </xsl:template> 

  <xsl:template name="name1">
    <BODY>
    <xsl:for-each select="R">
      <xsl:apply-templates select="A/B"/>
    </xsl:for-each>
    </BODY>
  </xsl:template> 

  <xsl:template match="A/B">
    <xsl:value-of select="C"/><BR/>
  </xsl:template> 

</xsl:stylesheet>
