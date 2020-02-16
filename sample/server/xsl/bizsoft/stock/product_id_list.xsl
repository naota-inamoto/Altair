<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <xsl:element name="SELECT">
      <xsl:attribute name="NAME">product_id</xsl:attribute>
      <xsl:attribute name="SIZE">1</xsl:attribute>
      <xsl:for-each select="商品マスター/商品">
        <xsl:if test="電子媒体='No'">
          <OPTION><xsl:value-of select="型番"/></OPTION>
        </xsl:if>
      </xsl:for-each>
    </xsl:element>
  </xsl:template> 

</xsl:stylesheet>
