<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
      <xsl:apply-templates select="勘定科目マスター/分類"/>
  </xsl:template> 

  <xsl:template match="勘定科目マスター/分類">
    <xsl:text>●</xsl:text>
    <xsl:value-of select="大分類"/>
    <xsl:if test="中分類">
      <xsl:text>-</xsl:text>
      <xsl:value-of select="中分類"/>
    </xsl:if>
    <xsl:if test="小分類">
      <xsl:text>-</xsl:text>
      <xsl:value-of select="小分類"/>
    </xsl:if>
    <BR/>
    <TABLE BORDER="1">
    <TR><TD>名称</TD><TD>読み</TD><TD>キャッシュ</TD><TD>貸借区分</TD><TD>借方税区分</TD><TD>貸方税区分</TD><TD>説明</TD></TR>
    <xsl:for-each select="勘定科目">
      <TR>
        <TD><xsl:value-of select="名称"/></TD>
        <TD><xsl:value-of select="読み"/></TD>
        <TD><xsl:value-of select="キャッシュ"/></TD>
        <TD><xsl:value-of select="貸借区分"/></TD>
        <TD><xsl:value-of select="借方税区分"/></TD>
        <TD><xsl:value-of select="貸方税区分"/></TD>
        <TD><xsl:value-of select="説明"/></TD>
      </TR>
    </xsl:for-each>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
