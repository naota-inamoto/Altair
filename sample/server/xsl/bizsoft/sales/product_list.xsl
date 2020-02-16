<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <TABLE BORDER="1">
      <xsl:for-each select="商品マスター/商品">
        <TR><TD>

          <xsl:text>商品名：</xsl:text>
          <xsl:value-of select="商品名"/><BR/>
          <xsl:text>型番：</xsl:text>
          <xsl:value-of select="型番"/><BR/>
          <xsl:value-of select="説明"/><BR/>

          <xsl:if test="電子媒体='No'">
            <xsl:text>単価：</xsl:text>
            <xsl:value-of select="単価"/>
            <xsl:text>円</xsl:text><BR/>
            <xsl:choose>
              <xsl:when test="詳細=''"/>
              <xsl:otherwise>
                <xsl:element name="A">
                  <xsl:attribute name="HREF"><xsl:value-of select="詳細"/></xsl:attribute>
                  <xsl:text>詳細</xsl:text>
                </xsl:element><BR/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:if>

          <xsl:if test="電子媒体='Yes'">
            <xsl:if test="ライセンス='No'">
              <xsl:choose>
                <xsl:when test="詳細=''"/>
                <xsl:otherwise>
                  <xsl:element name="A">
                    <xsl:attribute name="HREF"><xsl:value-of select="詳細"/></xsl:attribute>
                    <xsl:text>詳細</xsl:text>
                  </xsl:element><BR/>
                </xsl:otherwise>
              </xsl:choose>
              <xsl:choose>
                <xsl:when test="ダウンロード=''"/>
                <xsl:otherwise>
                  <xsl:element name="A">
                    <xsl:attribute name="HREF"><xsl:value-of select="ダウンロード"/></xsl:attribute>
                    <xsl:text>ダウンロード</xsl:text>
                  </xsl:element><BR/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:if>

            <xsl:if test="ライセンス='Yes'">
              <xsl:text>単価：</xsl:text>
              <xsl:value-of select="単価"/>
              <xsl:text>円</xsl:text><BR/>
            </xsl:if>
          </xsl:if>

          </TD><TD>

          <xsl:if test="電子媒体='No'">
            <xsl:element name="A">
              <xsl:attribute name="HREF">/products/ebiz?action=Cart&product=<xsl:value-of select="型番"/></xsl:attribute> 
              <xsl:text>カートに入れる</xsl:text>
            </xsl:element>
          </xsl:if>

          <xsl:if test="ライセンス='Yes'">
            <xsl:element name="A">
              <xsl:attribute name="HREF">/products/ebiz?action=Cart&product=<xsl:value-of select="型番"/></xsl:attribute> 
              <xsl:text>カートに入れる</xsl:text>
            </xsl:element>
          </xsl:if>

        </TD></TR>
      </xsl:for-each>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
