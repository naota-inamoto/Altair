<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
      <xsl:apply-templates select="����Ȗڃ}�X�^�[/����"/>
  </xsl:template> 

  <xsl:template match="����Ȗڃ}�X�^�[/����">
    <xsl:text>��</xsl:text>
    <xsl:value-of select="�啪��"/>
    <xsl:if test="������">
      <xsl:text>-</xsl:text>
      <xsl:value-of select="������"/>
    </xsl:if>
    <xsl:if test="������">
      <xsl:text>-</xsl:text>
      <xsl:value-of select="������"/>
    </xsl:if>
    <BR/>
    <TABLE BORDER="1">
    <TR><TD>����</TD><TD>�ǂ�</TD><TD>�L���b�V��</TD><TD>�ݎ؋敪</TD><TD>�ؕ��ŋ敪</TD><TD>�ݕ��ŋ敪</TD><TD>����</TD></TR>
    <xsl:for-each select="����Ȗ�">
      <TR>
        <TD><xsl:value-of select="����"/></TD>
        <TD><xsl:value-of select="�ǂ�"/></TD>
        <TD><xsl:value-of select="�L���b�V��"/></TD>
        <TD><xsl:value-of select="�ݎ؋敪"/></TD>
        <TD><xsl:value-of select="�ؕ��ŋ敪"/></TD>
        <TD><xsl:value-of select="�ݕ��ŋ敪"/></TD>
        <TD><xsl:value-of select="����"/></TD>
      </TR>
    </xsl:for-each>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
