<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <xsl:text>開始日付：</xsl:text>
    <xsl:value-of select="元帳/開始日付"/><BR/>
    <xsl:text>終了日付：</xsl:text>
    <xsl:value-of select="元帳/終了日付"/><BR/>
    <xsl:text>勘定科目：</xsl:text>
    <xsl:value-of select="元帳/科目名"/><BR/>
    <TABLE BORDER="1">
      <TR><TD>日付</TD><TD>自己補助科目</TD><TD>相手方勘定科目</TD><TD>相手方補助科目</TD>
          <TD>摘要</TD><TD>税区分</TD><TD>税率</TD><TD>借方</TD><TD>貸方</TD><TD>残高</TD></TR>
      <xsl:for-each select="元帳/項目">
        <TR>
          <TD><xsl:value-of select="日付"/></TD>
          <TD><xsl:value-of select="自己補助科目"/></TD>
          <TD><xsl:value-of select="相手方勘定科目"/></TD>
          <TD><xsl:value-of select="相手方補助科目"/></TD>
          <TD><xsl:value-of select="摘要"/></TD>
          <TD><xsl:value-of select="税区分"/></TD>
          <TD><xsl:value-of select="税率"/></TD>
          <TD><xsl:value-of select="借方"/></TD>
          <TD><xsl:value-of select="貸方"/></TD>
          <TD><xsl:value-of select="残高"/></TD>
        </TR>
      </xsl:for-each>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
