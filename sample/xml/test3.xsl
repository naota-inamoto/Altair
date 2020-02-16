<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
   <xsl:output method="xml" encoding="UTF-8" doctype-system="test.dtd"/>

  <xsl:template match="/">
    <HTML>
    <xsl:element name="HEAD">
      <xsl:element name="TITLE">Test</xsl:element>
    </xsl:element>
    <xsl:apply-templates select="R"/>
    </HTML>
  </xsl:template> 

  <xsl:template match="R">
    <BODY>
    <xsl:for-each select="A/B">
      <xsl:value-of select="C"/><BR/>
    </xsl:for-each>
    </BODY>
  </xsl:template> 

</xsl:stylesheet>
