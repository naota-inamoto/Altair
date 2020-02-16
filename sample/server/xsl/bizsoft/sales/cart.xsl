<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <TABLE BORDER="1">
      <TR><TD>商品名</TD><TD>型番</TD><TD>開始日</TD><TD>終了日</TD><TD>単価</TD><TD>数量</TD><TD>価格</TD></TR>
      <xsl:for-each select="EBIZ/商品">
        <TR>
          <TD><xsl:value-of select="商品名"/></TD>
          <TD><xsl:value-of select="型番"/></TD>
          <xsl:choose>
            <xsl:when test="ライセンス='Yes'">
              <TD><xsl:value-of select="開始日"/></TD>
              <TD><xsl:value-of select="終了日"/></TD>
            </xsl:when>
            <xsl:otherwise>
              <TD></TD>
              <TD></TD>
            </xsl:otherwise>
          </xsl:choose>
          <TD align="right"><xsl:value-of select="単価"/></TD>
          <TD>
            <xsl:element name="INPUT">
              <xsl:attribute name="TYPE">text</xsl:attribute>
              <xsl:attribute name="NAME"><xsl:value-of select="型番"/></xsl:attribute>
              <xsl:attribute name="SIZE">3</xsl:attribute>
              <xsl:attribute name="VALUE"><xsl:value-of select="数量"/></xsl:attribute>
            </xsl:element>
          </TD>
          <TD align="right"><xsl:value-of select="価格"/></TD>
        </TR>
      </xsl:for-each>
      <TR><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD><TD>小計</TD><TD align="right"><xsl:value-of select="EBIZ/小計"/></TD></TR>
      <TR><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD><TD>税金</TD><TD align="right"><xsl:value-of select="EBIZ/税金"/></TD></TR>
      <TR><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD><TD>送料</TD><TD align="right"><xsl:value-of select="EBIZ/送料"/></TD></TR>
      <TR><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD><TD>合計</TD><TD align="right"><xsl:value-of select="EBIZ/合計"/></TD></TR>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
