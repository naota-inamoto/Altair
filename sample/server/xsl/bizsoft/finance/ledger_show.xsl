<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <xsl:text>�J�n���t�F</xsl:text>
    <xsl:value-of select="����/�J�n���t"/><BR/>
    <xsl:text>�I�����t�F</xsl:text>
    <xsl:value-of select="����/�I�����t"/><BR/>
    <xsl:text>����ȖځF</xsl:text>
    <xsl:value-of select="����/�Ȗږ�"/><BR/>
    <TABLE BORDER="1">
      <TR><TD>���t</TD><TD>���ȕ⏕�Ȗ�</TD><TD>���������Ȗ�</TD><TD>������⏕�Ȗ�</TD>
          <TD>�E�v</TD><TD>�ŋ敪</TD><TD>�ŗ�</TD><TD>�ؕ�</TD><TD>�ݕ�</TD><TD>�c��</TD></TR>
      <xsl:for-each select="����/����">
        <TR>
          <TD><xsl:value-of select="���t"/></TD>
          <TD><xsl:value-of select="���ȕ⏕�Ȗ�"/></TD>
          <TD><xsl:value-of select="���������Ȗ�"/></TD>
          <TD><xsl:value-of select="������⏕�Ȗ�"/></TD>
          <TD><xsl:value-of select="�E�v"/></TD>
          <TD><xsl:value-of select="�ŋ敪"/></TD>
          <TD><xsl:value-of select="�ŗ�"/></TD>
          <TD><xsl:value-of select="�ؕ�"/></TD>
          <TD><xsl:value-of select="�ݕ�"/></TD>
          <TD><xsl:value-of select="�c��"/></TD>
        </TR>
      </xsl:for-each>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
