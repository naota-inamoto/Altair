<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <xsl:text>�`�[���t�F</xsl:text>
    <xsl:value-of select="�`�[/���t"/><BR/>
    <TABLE BORDER="1">
      <TR><TD>����</TD><TD>���[�U</TD><TD>���</TD><TD>����</TD></TR>
      <xsl:for-each select="�`�[/�`�F�b�N">
        <TR>
          <TD><xsl:value-of select="����"/></TD>
          <TD><xsl:value-of select="���[�U"/></TD>
          <TD><xsl:value-of select="���"/></TD>
          <TD><xsl:value-of select="����"/></TD>
        </TR>
      </xsl:for-each>
    </TABLE>
    <xsl:call-template name="�ؕ�"/>
    <xsl:call-template name="�ݕ�"/>
  </xsl:template> 

  <xsl:template name="�ؕ�">
    <xsl:text>�� �ؕ�</xsl:text><BR/>
    <TABLE BORDER="1">
      <TR>
        <TD>����Ȗ�</TD>
        <TD>�⏕�Ȗ�</TD>
        <TD>���z</TD>
        <TD>�Ŋz</TD>
        <TD>�ŋ敪</TD>
        <TD>�ŗ�</TD>
        <TD>�E�v</TD>
        <TD>����</TD>
        <TD>�t�</TD>
      </TR>
      <xsl:for-each select="�`�[/�d��/�ؕ�">
      <TR>
        <TD><xsl:value-of select="����Ȗ�"/></TD>
        <TD><xsl:value-of select="�⏕�Ȗ�"/></TD>
        <TD><xsl:value-of select="���z"/></TD>
        <TD><xsl:value-of select="�Ŋz"/></TD>
        <TD><xsl:value-of select="�ŋ敪"/></TD>
        <TD><xsl:value-of select="�ŗ�"/></TD>
        <TD><xsl:value-of select="�E�v"/></TD>
        <TD><xsl:value-of select="����"/></TD>
        <TD><xsl:value-of select="�t�"/></TD>
      </TR>
      </xsl:for-each>
    </TABLE>
  </xsl:template> 

  <xsl:template name="�ݕ�">
    <xsl:text>�� �ݕ�</xsl:text><BR/>
    <TABLE BORDER="1">
      <TR>
        <TD>����Ȗ�</TD>
        <TD>�⏕�Ȗ�</TD>
        <TD>���z</TD>
        <TD>�Ŋz</TD>
        <TD>�ŋ敪</TD>
        <TD>�ŗ�</TD>
        <TD>�E�v</TD>
        <TD>����</TD>
        <TD>�t�</TD>
      </TR>
      <xsl:for-each select="�`�[/�d��/�ݕ�">
        <TR>
          <TD><xsl:value-of select="����Ȗ�"/></TD>
          <TD><xsl:value-of select="�⏕�Ȗ�"/></TD>
          <TD><xsl:value-of select="���z"/></TD>
          <TD><xsl:value-of select="�Ŋz"/></TD>
          <TD><xsl:value-of select="�ŋ敪"/></TD>
          <TD><xsl:value-of select="�ŗ�"/></TD>
          <TD><xsl:value-of select="�E�v"/></TD>
          <TD><xsl:value-of select="����"/></TD>
          <TD><xsl:value-of select="�t�"/></TD>
        </TR>
      </xsl:for-each>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
