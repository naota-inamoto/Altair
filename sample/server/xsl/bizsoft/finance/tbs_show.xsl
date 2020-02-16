<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <xsl:text>開始日付：</xsl:text>
    <xsl:value-of select="ストック/開始日付"/><BR/>
    <xsl:text>終了日付：</xsl:text>
    <xsl:value-of select="ストック/終了日付"/><BR/>
    <TABLE BORDER="1">
      <TR><TD>大分類</TD><TD>中分類</TD><TD>小分類</TD><TD>勘定科目</TD>
          <TD>繰越</TD><TD>借方</TD><TD>貸方</TD><TD>残高</TD></TR>
      <xsl:for-each select="ストック/項目">
        <TR>
          <TD><xsl:value-of select="大分類"/></TD>
          <TD><xsl:value-of select="中分類"/></TD>
          <TD><xsl:value-of select="小分類"/></TD>
          <TD><xsl:value-of select="勘定科目"/></TD>
          <TD><xsl:value-of select="繰越"/></TD>
          <TD><xsl:value-of select="借方"/></TD>
          <TD><xsl:value-of select="貸方"/></TD>
          <TD><xsl:value-of select="残高"/></TD>
        </TR>
      </xsl:for-each>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
