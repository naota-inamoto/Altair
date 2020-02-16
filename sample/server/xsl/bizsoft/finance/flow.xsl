<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <xsl:text>期間：</xsl:text>
    <xsl:value-of select="フロー/期間"/><BR/>
    <TABLE BORDER="1">
      <TR><TD>大分類</TD><TD>中分類</TD><TD>小分類</TD><TD>勘定科目</TD><TD>金額</TD></TR>
      <xsl:for-each select="フロー/項目">
        <TR>
          <TD><xsl:value-of select="大分類"/></TD>
          <TD><xsl:value-of select="中分類"/></TD>
          <TD><xsl:value-of select="小分類"/></TD>
          <TD><xsl:value-of select="勘定科目"/></TD>
          <TD><xsl:value-of select="金額"/></TD>
        </TR>
      </xsl:for-each>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
