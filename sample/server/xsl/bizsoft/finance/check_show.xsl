<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <xsl:text>`[útF</xsl:text>
    <xsl:value-of select="`[/út"/><BR/>
    <TABLE BORDER="1">
      <TR><TD>ú</TD><TD>[U</TD><TD>íÊ</TD><TD></TD></TR>
      <xsl:for-each select="`[/`FbN">
        <TR>
          <TD><xsl:value-of select="ú"/></TD>
          <TD><xsl:value-of select="[U"/></TD>
          <TD><xsl:value-of select="íÊ"/></TD>
          <TD><xsl:value-of select=""/></TD>
        </TR>
      </xsl:for-each>
    </TABLE>
    <xsl:call-template name="Ĝû"/>
    <xsl:call-template name="Ŭû"/>
  </xsl:template> 

  <xsl:template name="Ĝû">
    <xsl:text> Ĝû</xsl:text><BR/>
    <TABLE BORDER="1">
      <TR>
        <TD>¨èÈÚ</TD>
        <TD>âÈÚ</TD>
        <TD>àz</TD>
        <TD>Ċz</TD>
        <TD>ĊĉŞ</TD>
        <TD>ĊĤ</TD>
        <TD>Ev</TD>
        <TD></TD>
        <TD>tâ³</TD>
      </TR>
      <xsl:for-each select="`[/dó/Ĝû">
      <TR>
        <TD><xsl:value-of select="¨èÈÚ"/></TD>
        <TD><xsl:value-of select="âÈÚ"/></TD>
        <TD><xsl:value-of select="àz"/></TD>
        <TD><xsl:value-of select="Ċz"/></TD>
        <TD><xsl:value-of select="ĊĉŞ"/></TD>
        <TD><xsl:value-of select="ĊĤ"/></TD>
        <TD><xsl:value-of select="Ev"/></TD>
        <TD><xsl:value-of select=""/></TD>
        <TD><xsl:value-of select="tâ³"/></TD>
      </TR>
      </xsl:for-each>
    </TABLE>
  </xsl:template> 

  <xsl:template name="Ŭû">
    <xsl:text> Ŭû</xsl:text><BR/>
    <TABLE BORDER="1">
      <TR>
        <TD>¨èÈÚ</TD>
        <TD>âÈÚ</TD>
        <TD>àz</TD>
        <TD>Ċz</TD>
        <TD>ĊĉŞ</TD>
        <TD>ĊĤ</TD>
        <TD>Ev</TD>
        <TD></TD>
        <TD>tâ³</TD>
      </TR>
      <xsl:for-each select="`[/dó/Ŭû">
        <TR>
          <TD><xsl:value-of select="¨èÈÚ"/></TD>
          <TD><xsl:value-of select="âÈÚ"/></TD>
          <TD><xsl:value-of select="àz"/></TD>
          <TD><xsl:value-of select="Ċz"/></TD>
          <TD><xsl:value-of select="ĊĉŞ"/></TD>
          <TD><xsl:value-of select="ĊĤ"/></TD>
          <TD><xsl:value-of select="Ev"/></TD>
          <TD><xsl:value-of select=""/></TD>
          <TD><xsl:value-of select="tâ³"/></TD>
        </TR>
      </xsl:for-each>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
