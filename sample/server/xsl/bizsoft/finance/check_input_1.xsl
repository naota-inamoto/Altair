<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <xsl:apply-templates select="EBIZ"/>
  </xsl:template> 

  <xsl:template match="EBIZ">
      <xsl:element name="INPUT">
        <xsl:attribute name="TYPE">hidden</xsl:attribute>
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_ah_type</xsl:attribute>
        <xsl:attribute name="VALUE"><xsl:value-of select="Type"/></xsl:attribute>
      </xsl:element>
      <xsl:variable name="selected" select="Selected"/>
      <xsl:element name="SELECT">
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_selected_ah</xsl:attribute>
        <xsl:for-each select="•ª—Þ/Š¨’è‰È–Ú">
          <xsl:element name="OPTION">
            <xsl:if test="–¼Ì=$selected">
              <xsl:attribute name="SELECTED">true</xsl:attribute>
            </xsl:if>
            <xsl:value-of select="–¼Ì"/>
          </xsl:element>
        </xsl:for-each>
      </xsl:element>
  </xsl:template> 

</xsl:stylesheet>
