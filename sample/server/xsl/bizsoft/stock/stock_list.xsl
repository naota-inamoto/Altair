<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <TABLE BORDER="1">
      <TR><TD>商品名</TD><TD>型番</TD><TD>電子媒体</TD><TD>在庫数</TD></TR>
      <xsl:for-each select="商品マスター/商品">
        <xsl:if test="電子媒体='No'">
          <TR>
            <TD><xsl:value-of select="商品名"/></TD>
            <TD><xsl:value-of select="型番"/></TD>
            <TD><xsl:value-of select="電子媒体"/></TD>
            <TD align="right"><xsl:value-of select="数量"/></TD>
          </TR>
        </xsl:if>
      </xsl:for-each>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
