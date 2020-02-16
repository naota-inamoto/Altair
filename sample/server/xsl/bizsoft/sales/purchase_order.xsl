<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <TABLE BORDER="1">
      <TR><TD>���i��</TD><TD>�^��</TD><TD>�J�n��</TD><TD>�I����</TD><TD>�P��</TD><TD>����</TD><TD>���i</TD></TR>
      <xsl:for-each select="����/����">
        <TR>
          <TD><xsl:value-of select="���i��"/></TD>
          <TD><xsl:value-of select="�^��"/></TD>
          <xsl:choose>
            <xsl:when test="�d�q�}��='Yes'">
              <TD><xsl:value-of select="�J�n��"/></TD>
              <TD><xsl:value-of select="�I����"/></TD>
            </xsl:when>
            <xsl:otherwise>
              <TD></TD>
              <TD></TD>
            </xsl:otherwise>
          </xsl:choose>
          <TD align="right"><xsl:value-of select="�P��"/></TD>
          <TD><xsl:value-of select="����"/></TD>
        </TR>
      </xsl:for-each>
      <TR><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD><TD>�ŋ�</TD><TD align="right"><xsl:value-of select="����/�ŋ�"/></TD></TR>
      <TR><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD><TD>����</TD><TD align="right"><xsl:value-of select="����/�z����"/></TD></TR>
      <TR><TD></TD><TD></TD><TD></TD><TD></TD><TD></TD><TD>���v</TD><TD align="right"><xsl:value-of select="����/�x���z"/></TD></TR>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
