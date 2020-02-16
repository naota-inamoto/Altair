<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <TABLE BORDER="1">
      <xsl:for-each select="���i�}�X�^�[/���i">
        <TR><TD>

          <xsl:text>���i���F</xsl:text>
          <xsl:value-of select="���i��"/><BR/>
          <xsl:text>�^�ԁF</xsl:text>
          <xsl:value-of select="�^��"/><BR/>
          <xsl:value-of select="����"/><BR/>

          <xsl:if test="�d�q�}��='No'">
            <xsl:text>�P���F</xsl:text>
            <xsl:value-of select="�P��"/>
            <xsl:text>�~</xsl:text><BR/>
            <xsl:choose>
              <xsl:when test="�ڍ�=''"/>
              <xsl:otherwise>
                <xsl:element name="A">
                  <xsl:attribute name="HREF"><xsl:value-of select="�ڍ�"/></xsl:attribute>
                  <xsl:text>�ڍ�</xsl:text>
                </xsl:element><BR/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:if>

          <xsl:if test="�d�q�}��='Yes'">
            <xsl:if test="���C�Z���X='No'">
              <xsl:choose>
                <xsl:when test="�ڍ�=''"/>
                <xsl:otherwise>
                  <xsl:element name="A">
                    <xsl:attribute name="HREF"><xsl:value-of select="�ڍ�"/></xsl:attribute>
                    <xsl:text>�ڍ�</xsl:text>
                  </xsl:element><BR/>
                </xsl:otherwise>
              </xsl:choose>
              <xsl:choose>
                <xsl:when test="�_�E�����[�h=''"/>
                <xsl:otherwise>
                  <xsl:element name="A">
                    <xsl:attribute name="HREF"><xsl:value-of select="�_�E�����[�h"/></xsl:attribute>
                    <xsl:text>�_�E�����[�h</xsl:text>
                  </xsl:element><BR/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:if>

            <xsl:if test="���C�Z���X='Yes'">
              <xsl:text>�P���F</xsl:text>
              <xsl:value-of select="�P��"/>
              <xsl:text>�~</xsl:text><BR/>
            </xsl:if>
          </xsl:if>

          </TD><TD>

          <xsl:if test="�d�q�}��='No'">
            <xsl:element name="A">
              <xsl:attribute name="HREF">/products/ebiz?action=Cart&product=<xsl:value-of select="�^��"/></xsl:attribute> 
              <xsl:text>�J�[�g�ɓ����</xsl:text>
            </xsl:element>
          </xsl:if>

          <xsl:if test="���C�Z���X='Yes'">
            <xsl:element name="A">
              <xsl:attribute name="HREF">/products/ebiz?action=Cart&product=<xsl:value-of select="�^��"/></xsl:attribute> 
              <xsl:text>�J�[�g�ɓ����</xsl:text>
            </xsl:element>
          </xsl:if>

        </TD></TR>
      </xsl:for-each>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
