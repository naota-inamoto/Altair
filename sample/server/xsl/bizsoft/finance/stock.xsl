<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <xsl:text>���t�F</xsl:text>
    <xsl:value-of select="�X�g�b�N/���t"/><BR/>
    <TABLE BORDER="1">
      <TR><TD>�啪��</TD><TD>������</TD><TD>������</TD><TD>����Ȗ�</TD><TD>���z</TD></TR>
      <xsl:for-each select="�X�g�b�N/����">
        <TR>
          <TD><xsl:value-of select="�啪��"/></TD>
          <TD><xsl:value-of select="������"/></TD>
          <TD><xsl:value-of select="������"/></TD>
          <TD><xsl:value-of select="����Ȗ�"/></TD>
          <TD><xsl:value-of select="���z"/></TD>
        </TR>
      </xsl:for-each>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
