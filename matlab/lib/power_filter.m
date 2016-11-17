function y = power_filter(p, th)

m_p = mean(p);
umbral_pot = m_p*th;
y = (p > umbral_pot);
