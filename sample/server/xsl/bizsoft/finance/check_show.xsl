<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <xsl:text>伝票日付：</xsl:text>
    <xsl:value-of select="伝票/日付"/><BR/>
    <TABLE BORDER="1">
      <TR><TD>日時</TD><TD>ユーザ</TD><TD>種別</TD><TD>メモ</TD></TR>
      <xsl:for-each select="伝票/チェック">
        <TR>
          <TD><xsl:value-of select="日時"/></TD>
          <TD><xsl:value-of select="ユーザ"/></TD>
          <TD><xsl:value-of select="種別"/></TD>
          <TD><xsl:value-of select="メモ"/></TD>
        </TR>
      </xsl:for-each>
    </TABLE>
    <xsl:call-template name="借方"/>
    <xsl:call-template name="貸方"/>
  </xsl:template> 

  <xsl:template name="借方">
    <xsl:text>● 借方</xsl:text><BR/>
    <TABLE BORDER="1">
      <TR>
        <TD>勘定科目</TD>
        <TD>補助科目</TD>
        <TD>金額</TD>
        <TD>税額</TD>
        <TD>税区分</TD>
        <TD>税率</TD>
        <TD>摘要</TD>
        <TD>メモ</TD>
        <TD>付箋</TD>
      </TR>
      <xsl:for-each select="伝票/仕訳/借方">
      <TR>
        <TD><xsl:value-of select="勘定科目"/></TD>
        <TD><xsl:value-of select="補助科目"/></TD>
        <TD><xsl:value-of select="金額"/></TD>
        <TD><xsl:value-of select="税額"/></TD>
        <TD><xsl:value-of select="税区分"/></TD>
        <TD><xsl:value-of select="税率"/></TD>
        <TD><xsl:value-of select="摘要"/></TD>
        <TD><xsl:value-of select="メモ"/></TD>
        <TD><xsl:value-of select="付箋"/></TD>
      </TR>
      </xsl:for-each>
    </TABLE>
  </xsl:template> 

  <xsl:template name="貸方">
    <xsl:text>● 貸方</xsl:text><BR/>
    <TABLE BORDER="1">
      <TR>
        <TD>勘定科目</TD>
        <TD>補助科目</TD>
        <TD>金額</TD>
        <TD>税額</TD>
        <TD>税区分</TD>
        <TD>税率</TD>
        <TD>摘要</TD>
        <TD>メモ</TD>
        <TD>付箋</TD>
      </TR>
      <xsl:for-each select="伝票/仕訳/貸方">
        <TR>
          <TD><xsl:value-of select="勘定科目"/></TD>
          <TD><xsl:value-of select="補助科目"/></TD>
          <TD><xsl:value-of select="金額"/></TD>
          <TD><xsl:value-of select="税額"/></TD>
          <TD><xsl:value-of select="税区分"/></TD>
          <TD><xsl:value-of select="税率"/></TD>
          <TD><xsl:value-of select="摘要"/></TD>
          <TD><xsl:value-of select="メモ"/></TD>
          <TD><xsl:value-of select="付箋"/></TD>
        </TR>
      </xsl:for-each>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
