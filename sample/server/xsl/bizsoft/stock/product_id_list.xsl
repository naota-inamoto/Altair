<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <xsl:element name="SELECT">
      <xsl:attribute name="NAME">product_id</xsl:attribute>
      <xsl:attribute name="SIZE">1</xsl:attribute>
      <xsl:for-each select="���i�}�X�^�[/���i">
        <xsl:if test="�d�q�}��='No'">
          <OPTION><xsl:value-of select="�^��"/></OPTION>
        </xsl:if>
      </xsl:for-each>
    </xsl:element>
  </xsl:template> 

</xsl:stylesheet>
